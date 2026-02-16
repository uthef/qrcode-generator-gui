#include "mainwindow.hpp"
#include <QDir>
#include <QDesktopServices>
#include <QFileDialog>
#include <qr/qrcodegenrator.hpp>
#include <QPainter>
#include <QColorDialog>
#include "widgets/popupdialog.hpp"
#include <QSvgRenderer>

using namespace QrCodeGeneratorGui;

MainWindow::MainWindow(QString defaultImagePath, QWidget *parent, Qt::WindowFlags flags) : 
    QMainWindow(parent, flags),
    layout(this)
{
    setWindowTitle(QString(APP_TITLE) + " (v" + QString(APP_VERSION) + ")");
    setMinimumSize(QSize(740, 740));

    layout.fileExtensionMenu()->addItems(fileFormats);
    layout.errorCorrectionMenu()->addItems(errorCorrectionLevels);

    layout.borderSizeSlider()->setRange(0, 50);
    layout.borderSizeSlider()->setValue(0);

    layout.imageSizeSlider()->setRange(256, 2048);
    layout.imageSizeSlider()->setSingleStep(8);

    layout.logoSizeSlider()->setRange(0, 30);
    layout.logoSizeSlider()->setValue(1);

    layout.previewCheckBox()->setChecked(true);

    layout.backgroundColorButton()->setText(backgroundColor.name(QColor::HexArgb));
    layout.foregroundColorButton()->setText(foregroundColor.name(QColor::HexArgb));

    connect(layout.fileDirectoryOpenButton(), &QPushButton::clicked, this, &MainWindow::onFileDirectoryOpenButtonClicked);
    connect(layout.fileDirectoryBrowseButton(), &QPushButton::clicked, this, &MainWindow::onFileDirectoryBrowseButtonClicked);
    connect(layout.logoEnabledCheckBox(), &QCheckBox::checkStateChanged, this, &MainWindow::onLogoEnabledCheckBoxStateChanged);
    connect(layout.contentField(), &QTextEdit::textChanged, this, &MainWindow::onContentFieldTextChanged);
    connect(layout.errorCorrectionMenu(), &QComboBox::currentTextChanged, this, &MainWindow::onErrorCorrectionLevelChanged);
    connect(layout.borderSizeSlider(), &QSlider::valueChanged, this, &MainWindow::onBorderSizeSliderValueChanged);
    connect(layout.imageSizeSlider(), &QSlider::valueChanged, this, &MainWindow::onImageSizeSliderValueChanged);
    connect(layout.foregroundColorButton(), &QPushButton::clicked, this, &MainWindow::onForegroundColorButtonClicked);
    connect(layout.backgroundColorButton(), &QPushButton::clicked, this, &MainWindow::onBackgroundColorButtonClicked);
    connect(layout.resetColorsButton(), &QPushButton::clicked, this, &MainWindow::onResetColorsButtonClicked);
    connect(layout.saveButton(), &QPushButton::clicked, this, &MainWindow::onSaveButtonClicked);
    connect(layout.logoSizeSlider(), &QSlider::valueChanged, this, &MainWindow::onLogoSizeSliderValueChanged);
    connect(layout.changeLogoButton(), &QPushButton::clicked, this, &MainWindow::onChangeLogoButtonClicked);
    connect(layout.previewCheckBox(), &QCheckBox::checkStateChanged, this, &MainWindow::onPreviewCheckBoxStateChanged);
    
    layout.borderSizeSlider()->setValue(10);
    layout.imageSizeSlider()->setValue(512);
    layout.logoSizeSlider()->setValue(0);
    layout.contentField()->setText("Sample text");

    layout.fileDirectoryField()->setText(defaultImagePath);
    layout.fileNameField()->setText("qrcode");

    layout.logoEnabledCheckBox()->setCheckState(Qt::CheckState::Unchecked);
    setLogoControlsEnabled(false);

    regenerateQrCode(qrCodePreviewImageData);
    updateQrCodeImage();

    preventGenerations = false;
}

void MainWindow::setLogoControlsEnabled(bool value)
{
    layout.logoFrame()->setEnabled(value);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    updateQrCodeImage();
}

bool MainWindow::regenerateQrCode(QImage &image, bool preview)
{
    if (!layout.previewCheckBox()->isChecked() && preview)
        return false;

    QString eccString = layout.errorCorrectionMenu()->currentText();
    image = QImage(512, 512, defaultImageFormat);

    QPainter painter(&image);

    QString contentFieldText = layout.contentField()->toPlainText();
    std::string contentFieldStdString = contentFieldText.toStdString();
    const char *contentFieldCString = contentFieldStdString.c_str();

    auto *logo = &logoImageData;

    if (logoImageData.isNull() || !layout.logoEnabledCheckBox()->isChecked()) 
        logo = nullptr;

    try {
        QrCodeGenerator::encodeTextAsImage(
            contentFieldCString, 
            QrCodeGenerator::mapEcc(eccString), 
            painter,
            foregroundColor,
            backgroundColor,
            borderSize,
            layout.logoSizeSlider()->value(),
            logo);
    }
    catch (const qrcodegen::data_too_long &e) {
        paintErrorImage(painter);
        showErrorPopup(tr("QR code text is too long.") + "\n" + QString(e.what()));
        return false;
    }
    catch (const std::exception &e) {
        paintErrorImage(painter);
        showErrorPopup(tr("QR code image generation failed"));
        return false;
    }

    return true;
}

void MainWindow::updateQrCodeImage()
{
    if (!layout.previewCheckBox()->isChecked())
        return;

    QPixmap pixmap = QPixmap::fromImage(qrCodePreviewImageData);

    auto margins = layout.qrPreviewImage()->contentsMargins();

    int width = qMin(
        layout.qrPreviewImage()->width(), 
        (int)(qrCodePreviewImageData.size().width() * 1.5)) - margins.left() - margins.right();

    int height = qMin(
        layout.qrPreviewImage()->height(), 
        (int)(qrCodePreviewImageData.size().width() * 1.5)) - margins.top() - margins.bottom();

    pixmap = pixmap.scaled(
        width, 
        height, 
        Qt::KeepAspectRatio, 
        Qt::TransformationMode::FastTransformation);

    layout.qrPreviewImage()->setPixmap(pixmap);
}

void MainWindow::showErrorPopup(QString text)
{
    PopupDialog popupDialog(this, text, false, PopupDialog::ERROR);
    popupDialog.exec();
}

void MainWindow::paintErrorImage(QPainter &painter)
{
    painter.fillRect(
        QRectF(0, 0, painter.window().width(), painter.window().height()), 
        QBrush(backgroundColor, Qt::SolidPattern));
    
    QSizeF size(painter.window().width() / 3, painter.window().width() / 3);

    QRectF bounds = QRectF(
        painter.window().width() / 2 - size.width() / 2, 
        painter.window().height() / 2 - size.height() / 2, 
        size.width(), 
        size.height());

    QString filename = ":/images/error.svg";
    QSvgRenderer svgRenderer(filename);
    svgRenderer.render(&painter, bounds);
}

void MainWindow::onFileDirectoryOpenButtonClicked()
{
    QDesktopServices::openUrl(layout.fileDirectoryField()->text());
}

void MainWindow::onFileDirectoryBrowseButtonClicked()
{
    QFileDialog fileDialog;

    fileDialog.setWindowTitle(tr("Select a directory"));
    fileDialog.setFileMode(QFileDialog::FileMode::Directory);
    fileDialog.setDirectory(QString(layout.fileDirectoryField()->text()));
    fileDialog.exec();

    if (fileDialog.result())
        layout.fileDirectoryField()->setText(fileDialog.directory().absolutePath());
}

void MainWindow::onLogoEnabledCheckBoxStateChanged(Qt::CheckState state)
{
    setLogoControlsEnabled(state == Qt::Checked);

    const auto prevExtension = layout.fileExtensionMenu()->currentText();
    const auto prevLevel = layout.errorCorrectionMenu()->currentText();

    layout.fileExtensionMenu()->clear();
    layout.fileExtensionMenu()->addItems(state == Qt::Checked ? rasterFileFormats : fileFormats);
    layout.fileExtensionMenu()->setCurrentText(prevExtension);

    layout.errorCorrectionMenu()->clear();
    layout.errorCorrectionMenu()->addItems(state == Qt::Checked ? alternateErrorCorrectionLevels : errorCorrectionLevels);
    layout.errorCorrectionMenu()->setCurrentText(prevLevel);

    regenerateQrCode(qrCodePreviewImageData);
    updateQrCodeImage();
}

void MainWindow::onContentFieldTextChanged()
{
    if (preventGenerations)
        return;

    regenerateQrCode(qrCodePreviewImageData);
    updateQrCodeImage();
}

void MainWindow::onBorderSizeSliderValueChanged(int value)
{
    borderSize = value;
    layout.borderSizeValueLabel()->setText(QString::number(value).append('%'));

    if (preventGenerations)
            return;

    regenerateQrCode(qrCodePreviewImageData);
    updateQrCodeImage();
}

void MainWindow::onImageSizeSliderValueChanged(int value)
{
    auto singleStep = layout.imageSizeSlider()->singleStep();

    if (value % singleStep != 0)
        value = value + (singleStep - value % singleStep);

    imageSize = value;
    layout.imageSizeValueLabel()->setText(QString::number(imageSize).append("px"));
}

void MainWindow::onErrorCorrectionLevelChanged()
{
    if (preventGenerations)
        return;

    regenerateQrCode(qrCodePreviewImageData);
    updateQrCodeImage();
}

void MainWindow::onForegroundColorButtonClicked()
{
    QColorDialog colorDialog;
    colorDialog.setWindowTitle(tr("Select foreground color"));
    colorDialog.setOptions(QColorDialog::ShowAlphaChannel);
    colorDialog.setCurrentColor(foregroundColor);
    colorDialog.exec();

    if (colorDialog.result()) {
        foregroundColor = colorDialog.selectedColor();
        layout.foregroundColorButton()->setText(foregroundColor.name(QColor::HexArgb));

        regenerateQrCode(qrCodePreviewImageData);
        updateQrCodeImage();
    }
}

void MainWindow::onBackgroundColorButtonClicked()
{
    QColorDialog colorDialog;
    colorDialog.setWindowTitle(tr("Select background color"));
    colorDialog.setOptions(QColorDialog::ShowAlphaChannel);
    colorDialog.setCurrentColor(backgroundColor);
    colorDialog.exec();

    if (colorDialog.result()) {
        backgroundColor = colorDialog.selectedColor();
        layout.backgroundColorButton()->setText(backgroundColor.name(QColor::HexArgb));

        regenerateQrCode(qrCodePreviewImageData);
        updateQrCodeImage();
    }
}

void MainWindow::onResetColorsButtonClicked()
{
    foregroundColor = QColor(255, 255, 255, 255);
    backgroundColor = QColor(20, 20, 20, 255);

    layout.foregroundColorButton()->setText(foregroundColor.name(QColor::HexArgb));
    layout.backgroundColorButton()->setText(backgroundColor.name(QColor::HexArgb));

    regenerateQrCode(qrCodePreviewImageData);
    updateQrCodeImage();
}

void MainWindow::onLogoSizeSliderValueChanged(int value)
{
    layout.logoSizeValueLabel()->setText(QString::number(value) + "%");
    regenerateQrCode(qrCodePreviewImageData);
    updateQrCodeImage();
}

void MainWindow::onChangeLogoButtonClicked()
{
    QFileDialog fileDialog;
    fileDialog.setWindowTitle(tr("Change logo image"));
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("*.png *.jpg *.webp *.svg");

    if (layout.logoPathField()->text().isEmpty()) {
        fileDialog.setDirectory(layout.fileDirectoryField()->text());
    }
    else {
        fileDialog.setDirectory("");
        fileDialog.selectFile(layout.logoPathField()->text());
    }

    fileDialog.exec();

    if (!fileDialog.result() || fileDialog.selectedFiles().isEmpty())
        return;

    const auto filePath = fileDialog.selectedFiles().first();

    if (filePath.isEmpty() || !QFile(filePath).exists()) {
        showErrorPopup(tr("File does not exist"));
        return;
    }

    layout.logoPathField()->setText(filePath);

    logoImageData = QImage(512, 512, defaultImageFormat);

    if (!logoImageData.load(filePath)) {
        logoImageData = QImage();
        showErrorPopup(tr("Unable to load logo image"));
        return;
    }

    if (layout.logoSizeSlider()->value() > 0) {
        regenerateQrCode(qrCodePreviewImageData);
        updateQrCodeImage();
    }
}

void MainWindow::onSaveButtonClicked()
{
    QString fileName = layout.fileNameField()->text();
    QString extension = layout.fileExtensionMenu()->currentText();
    QDir dir(layout.fileDirectoryField()->text());
    QString fullFilePath = dir.absoluteFilePath(fileName + extension);

    if (fileName.isEmpty() || dir.isEmpty()) 
    {
        showErrorPopup(tr("File name cannot be empty"));
        return;
    }

    if (extension == ".jpg" && (foregroundColor.alpha() < 255 || backgroundColor.alpha() < 255)) {
        PopupDialog popup(
            this,
            tr("You have selected colors with transparency.\nNote that JPEG format does not preserve an alpha channel."),
             false,
            PopupDialog::WARNING
        );
        
        popup.exec();
    }

    if (fileName.contains('\\') || fileName.contains('/')) {
        showErrorPopup(tr("Output file name contains invalid characters"));
        return;
    }

    if (!dir.exists()) {
        showErrorPopup(tr("The directory you have selected does not exist"));
        return;
    }

    QFile outputFile(fullFilePath);

    if (outputFile.exists()) {
        PopupDialog popupDialog(
            this,
            tr("A file with the chosen name already exists.\nDo you really want to overwrite it?"), 
            true,
            PopupDialog::WARNING);

        popupDialog.exec();

        if (!popupDialog.result())
            return;
    }

    if (!outputFile.open(QIODeviceBase::WriteOnly)) {
        showErrorPopup(tr("Sorry, could not open the specified file for writing"));
        return;
    }

    auto exception = false;
    QString dataString = layout.contentField()->toPlainText();
    std::string dataStdString = dataString.toStdString();
    auto ecc = QrCodeGenerator::mapEcc(layout.errorCorrectionMenu()->currentText());

    if (extension == ".svg") {
        try {
            auto svg = QrCodeGenerator::encodeTextAsSvgString(
                dataStdString.c_str(),
                ecc,
                foregroundColor,
                backgroundColor,
                borderSize);
            
            outputFile.write(svg);
            outputFile.flush();
        }
        catch (const qrcodegen::data_too_long &e) {
            showErrorPopup(tr("QR code text is too long.") + "\n" + QString(e.what()));
            exception = true;
        }
        catch (const std::exception &e) {
            showErrorPopup(tr("QR code image generation failed"));
            exception = true;
        }
    }
    else {
        QImage image(imageSize, imageSize, defaultImageFormat);
        
        if (!regenerateQrCode(image, false)) {
            exception = true;
        }
        else {
            std::string format = extension.removeFirst().toStdString();

            if (!image.save(&outputFile, format.c_str())) {
                showErrorPopup(tr("The program failed to save this image"));
                exception = true;
            }
        }
    }  

    outputFile.close();

    if (exception)
        return;

    PopupDialog popupDialog(
        this, 
        tr("You have successfully exported an image!"),
        false,
        PopupDialog::SUCCESS);
        
    popupDialog.exec();

    if (layout.openAfterExportCheckBox()->isChecked())
        QDesktopServices::openUrl(fullFilePath);
}

void MainWindow::onPreviewCheckBoxStateChanged(Qt::CheckState state)
{
    layout.qrPreviewFrame()->setVisible(state == Qt::Checked);

    if (state == Qt::Checked) {
        regenerateQrCode(qrCodePreviewImageData);
        updateQrCodeImage();
    }
}