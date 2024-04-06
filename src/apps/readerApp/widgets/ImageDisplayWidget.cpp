#include "widgets/ImageDisplayWidget.hpp"

#include <QPainter>

namespace app {
namespace ui {

ImageDisplayWidget::ImageDisplayWidget(QQuickItem* const parent)
  : QQuickPaintedItem(parent)
{
    QObject::connect(this, &ImageDisplayWidget::imageChanged, [&]() { update(); });
}

void ImageDisplayWidget::paint(QPainter* const painter)
{
    if(painter && !m_image.isNull())
    {
        if(m_image.isNull())
        {
            painter->eraseRect(0, 0, static_cast<int>(width()), static_cast<int>(height()));
            return;
        }

        QImage scaled;

        if(m_image.width() > m_image.height())
        {
            scaled = m_image.scaledToWidth(static_cast<int>(width()));
            if(height() < scaled.height())
            {
                scaled = m_image.scaledToHeight(static_cast<int>(height()));
            }
        }
        else
        {
            scaled = m_image.scaledToHeight(static_cast<int>(height()));
            if(width() < scaled.width())
            {
                scaled = m_image.scaledToWidth(static_cast<int>(width()));
            }
        }

        const QPoint topL(static_cast<int>((width() - scaled.width()) / 2.0),
                          static_cast<int>((height() - scaled.height()) / 2.0));
        const QPoint bottomR(static_cast<int>(width() / 2.0 + scaled.width() / 2.0),
                             static_cast<int>(height() / 2.0 + scaled.height() / 2.0));

        painter->drawImage(topL, scaled);
    }
}

QVariant ImageDisplayWidget::getImage() const { return QVariant::fromValue(m_image); }

void ImageDisplayWidget::setImage(QVariant image)
{
    if(!image.isValid() && !image.isNull())
    {
        m_image = QImage{};
    }
    else
    {
        const auto extractedImage = image.value<QImage>();

        if(extractedImage == m_image)
        {
            return;
        }

        m_image = extractedImage;
    }

    emit imageChanged();
}

}
}