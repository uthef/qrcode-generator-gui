#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <layouts/mainwindowlayout.hpp>

namespace QrCodeGeneratorGui
{

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QString appPath, QWidget *parent = nullptr, Qt::WindowFlags flags = {{{0U}}});
private:
    MainWindowLayout layout;
    QImage qrCodePreviewImageData;
    QImage logoImageData;

    QImage::Format defaultImageFormat = QImage::Format::Format_ARGB32;
    float borderSize = 0;
    int imageSize = 256;
    QColor foregroundColor = QColor(255, 255, 255, 255);
    QColor backgroundColor = QColor(20, 20, 20, 255);

    const QStringList fileFormats = {
        ".png",
        ".jpg",
        ".svg",
        ".webp"
    };

    const QStringList rasterFileFormats = {
        ".png",
        ".jpg",
        ".webp"
    };

    const QStringList errorCorrectionLevels = {
        "L (7%)",
        "M (15%)",
        "Q (25%)",
        "H (30%)"
    };

    const QStringList alternateErrorCorrectionLevels = {
        "Q (25%)",
        "H (30%)"
    };

    bool preventGenerations = true;

    void setLogoControlsEnabled(bool value);
    void resizeEvent(QResizeEvent *event) override;
    bool regenerateQrCode(QImage &image, bool preview = true);
    void updateQrCodeImage();
    void showErrorPopup(QString text);
    void paintErrorImage(QPainter &painter);

private slots:
    void onFileDirectoryOpenButtonClicked();
    void onFileDirectoryBrowseButtonClicked();
    void onLogoEnabledCheckBoxStateChanged(Qt::CheckState);
    void onContentFieldTextChanged();
    void onBorderSizeSliderValueChanged(int value);
    void onImageSizeSliderValueChanged(int value);
    void onErrorCorrectionLevelChanged();
    void onForegroundColorButtonClicked();
    void onBackgroundColorButtonClicked();
    void onResetColorsButtonClicked();
    void onLogoSizeSliderValueChanged(int value);
    void onChangeLogoButtonClicked();
    void onSaveButtonClicked();
    void onPreviewCheckBoxStateChanged(Qt::CheckState);
};

}

#endif