/*
 * Copyright (C) 2006 Alexander Kellett <lypanov@kde.org>
 * Copyright (C) 2006 Apple Computer, Inc.
 * Copyright (C) 2007 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2007, 2008, 2009 Rob Buis <buis@kde.org>
 * Copyright (C) 2009 Google, Inc.
 * Copyright (C) 2009 Dirk Schulze <krit@webkit.org>
 * Copyright (C) 2010 Patrick Gansterer <paroga@paroga.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"

#include "core/rendering/svg/RenderSVGImage.h"

#include "core/paint/SVGImagePainter.h"
#include "core/rendering/ImageQualityController.h"
#include "core/rendering/PointerEventsHitRules.h"
#include "core/rendering/RenderImageResource.h"
#include "core/rendering/svg/RenderSVGResourceContainer.h"
#include "core/rendering/svg/SVGRenderSupport.h"
#include "core/rendering/svg/SVGResources.h"
#include "core/rendering/svg/SVGResourcesCache.h"
#include "core/svg/SVGImageElement.h"
#include "platform/LengthFunctions.h"

namespace blink {

RenderSVGImage::RenderSVGImage(SVGImageElement* impl)
    : RenderSVGModelObject(impl)
    , m_needsBoundariesUpdate(true)
    , m_needsTransformUpdate(true)
    , m_imageResource(RenderImageResource::create())
{
    m_imageResource->initialize(this);
}

RenderSVGImage::~RenderSVGImage()
{
}

void RenderSVGImage::destroy()
{
    ImageQualityController::remove(this);
    m_imageResource->shutdown();
    RenderSVGModelObject::destroy();
}

bool RenderSVGImage::forceNonUniformScaling(SVGImageElement* image) const
{
    // Images with preserveAspectRatio=none should force non-uniform
    // scaling. This can be achieved by setting the image's container size to
    // its intrinsic size. If the image does not have an intrinsic size - or
    // the intrinsic size is degenerate - set the container size to the bounds
    // as in pAR!=none cases.
    // See: http://www.w3.org/TR/SVG/single-page.html, 7.8 The ‘preserveAspectRatio’ attribute.
    if (image->preserveAspectRatio()->currentValue()->align() != SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_NONE)
        return false;
    ImageResource* cachedImage = m_imageResource->cachedImage();
    if (!cachedImage)
        return false;
    Length intrinsicWidth;
    Length intrinsicHeight;
    FloatSize intrinsicRatio;
    cachedImage->computeIntrinsicDimensions(intrinsicWidth, intrinsicHeight, intrinsicRatio);
    if (!intrinsicWidth.isFixed() || !intrinsicHeight.isFixed())
        return false;
    // If the viewport defined by the referenced image is zero in either
    // dimension, then SVGImage will have computed an intrinsic size of 300x150.
    if (!floatValueForLength(intrinsicWidth, 0) || !floatValueForLength(intrinsicHeight, 0))
        return false;
    return true;
}

bool RenderSVGImage::updateImageViewport()
{
    SVGImageElement* image = toSVGImageElement(element());
    FloatRect oldBoundaries = m_objectBoundingBox;
    bool updatedViewport = false;

    SVGLengthContext lengthContext(image);
    m_objectBoundingBox = FloatRect(image->x()->currentValue()->value(lengthContext), image->y()->currentValue()->value(lengthContext), image->width()->currentValue()->value(lengthContext), image->height()->currentValue()->value(lengthContext));

    bool boundsChanged = oldBoundaries != m_objectBoundingBox;

    IntSize newViewportSize;
    if (forceNonUniformScaling(image)) {
        LayoutSize intrinsicSize = m_imageResource->intrinsicSize(style()->effectiveZoom());
        if (intrinsicSize != m_imageResource->imageSize(style()->effectiveZoom())) {
            newViewportSize = roundedIntSize(intrinsicSize);
            updatedViewport = true;
        }
    } else if (boundsChanged) {
        newViewportSize = enclosingIntRect(m_objectBoundingBox).size();
        updatedViewport = true;
    }
    if (updatedViewport)
        m_imageResource->setContainerSizeForRenderer(newViewportSize);
    m_needsBoundariesUpdate |= boundsChanged;
    return updatedViewport;
}

void RenderSVGImage::layout()
{
    ASSERT(needsLayout());

    updateImageViewport();

    bool transformOrBoundariesUpdate = m_needsTransformUpdate || m_needsBoundariesUpdate;
    if (m_needsTransformUpdate) {
        m_localTransform = toSVGImageElement(element())->animatedLocalTransform();
        m_needsTransformUpdate = false;
    }

    if (m_needsBoundariesUpdate) {
        m_paintInvalidationBoundingBox = m_objectBoundingBox;
        SVGRenderSupport::intersectPaintInvalidationRectWithResources(this, m_paintInvalidationBoundingBox);

        m_needsBoundariesUpdate = false;
    }

    // Invalidate all resources of this client if our layout changed.
    if (everHadLayout() && selfNeedsLayout())
        SVGResourcesCache::clientLayoutChanged(this);

    // If our bounds changed, notify the parents.
    if (transformOrBoundariesUpdate)
        RenderSVGModelObject::setNeedsBoundariesUpdate();

    clearNeedsLayout();
}

void RenderSVGImage::paint(PaintInfo& paintInfo, const LayoutPoint&)
{
    SVGImagePainter(*this).paint(paintInfo);
}

void RenderSVGImage::invalidateBufferedForeground()
{
    m_bufferedForeground.clear();
}

bool RenderSVGImage::nodeAtFloatPoint(const HitTestRequest& request, HitTestResult& result, const FloatPoint& pointInParent, HitTestAction hitTestAction)
{
    // We only draw in the forground phase, so we only hit-test then.
    if (hitTestAction != HitTestForeground)
        return false;

    PointerEventsHitRules hitRules(PointerEventsHitRules::SVG_IMAGE_HITTESTING, request, style()->pointerEvents());
    bool isVisible = (style()->visibility() == VISIBLE);
    if (isVisible || !hitRules.requireVisible) {
        FloatPoint localPoint;
        if (!SVGRenderSupport::transformToUserSpaceAndCheckClipping(this, localToParentTransform(), pointInParent, localPoint))
            return false;

        if (hitRules.canHitFill || hitRules.canHitBoundingBox) {
            if (m_objectBoundingBox.contains(localPoint)) {
                updateHitTestResult(result, roundedLayoutPoint(localPoint));
                return true;
            }
        }
    }

    return false;
}

void RenderSVGImage::imageChanged(WrappedImagePtr, const IntRect*)
{
    // The image resource defaults to nullImage until the resource arrives.
    // This empty image may be cached by SVG resources which must be invalidated.
    if (SVGResources* resources = SVGResourcesCache::cachedResourcesForRenderObject(this))
        resources->removeClientFromCache(this);

    // Eventually notify parent resources, that we've changed.
    RenderSVGResourceContainer::markForLayoutAndParentResourceInvalidation(this, false);

    // Update the SVGImageCache sizeAndScales entry in case image loading finished after layout.
    // (https://bugs.webkit.org/show_bug.cgi?id=99489)
    m_objectBoundingBox = FloatRect();
    updateImageViewport();

    invalidateBufferedForeground();

    setShouldDoFullPaintInvalidation();
}

void RenderSVGImage::addFocusRingRects(Vector<LayoutRect>& rects, const LayoutPoint&, const RenderLayerModelObject*) const
{
    // this is called from paint() after the localTransform has already been applied
    LayoutRect contentRect = LayoutRect(paintInvalidationRectInLocalCoordinates());
    if (!contentRect.isEmpty())
        rects.append(contentRect);
}

} // namespace blink
