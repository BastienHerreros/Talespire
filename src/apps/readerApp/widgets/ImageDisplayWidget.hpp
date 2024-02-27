#pragma once

#include <QImage>
#include <QQuickPaintedItem>
#include <QVariant>

namespace app {
namespace ui {

/**
 * @class ImageDisplayWidget
 * @brief Display a QImage
 */
class ImageDisplayWidget : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QVariant image READ getImage WRITE setImage NOTIFY imageChanged)

  public:
    /**
     * @brief Construct a new ImageDisplayWidget object
     * @param [in] parent the parent
     */
    ImageDisplayWidget(QQuickItem* const parent = nullptr);

    /**
     * @brief Destroy the ImageDisplayWidget object
     */
    virtual ~ImageDisplayWidget() override = default;

    /**
     * @brief Updates the display
     * @param [in] painter the painter used to draw
     */
    virtual void paint(QPainter* const painter) final;

    /**
     * @brief Get the displayed image
     * @return The displayed image
     */
    QVariant getImage() const;

    /**
     *
     * @brief Set the image to display
     * @param [in] image the image to display
     */
    void setImage(QVariant image);

  signals:

    /**
     * @brief Signal when the image is updated
     */
    void imageChanged() const;

  private:
    /// The displayed image
    QImage m_image;
};
}
}