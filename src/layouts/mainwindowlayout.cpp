#include "mainwindowlayout.hpp"
#include <QSizePolicy>
#include <QApplication>
#include <QScrollArea>

using namespace QrCodeGeneratorGui;

MainWindowLayout::MainWindowLayout(QMainWindow *window) : 
    mLayout(QBoxLayout::LeftToRight, &mCentralWidget),
    mBorderSizeSlider(Qt::Orientation::Horizontal),
    mImageSizeSlider(Qt::Orientation::Horizontal),
    mLogoSizeSlider(Qt::Orientation::Horizontal),
    paletteIcon(":/images/palette.svg")
{
    if (window == nullptr) {
        qDebug() << "MainWindowLayout:" << "window pointer cannot be null";
        return;
    }
    
    mWindow = window;
    this->window()->setCentralWidget(&mCentralWidget);

    mLayout.setSpacing(8);

    mControlLayout.setAlignment(Qt::AlignTop);

    addOutputFileDirectoryWidgets();
    addOutputFileNameWidgets();
    addContentWidgets();
    addAdvancedOptionsWidgets();
    addCheckBoxes();
    addFooter();

    mQrPreviewImage.setBackgroundRole(QPalette::Dark);
    mQrPreviewImage.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mQrPreviewImage.setAlignment(Qt::AlignCenter);
    mQrPreviewImage.setContentsMargins(QMargins(16, 16, 16, 16));

    mQrPreviewLayout.addWidget(&mQrPreviewImage);

    mQrPreviewFrame.setLayout(&mQrPreviewLayout);
    auto windowColor = QApplication::palette().color(QPalette::Window).darker(120);
    mQrPreviewFrame.setStyleSheet(
        QString("background-color: %1; border-radius: 8px;")
            .arg(windowColor.name()));

    mLayout.addLayout(&mControlLayout);
    mLayout.addWidget(&mQrPreviewFrame);
}

QMainWindow *MainWindowLayout::window() const 
{
    return mWindow;
}

QLabel *MainWindowLayout::qrPreviewImage()
{
    return &mQrPreviewImage;
}

QFrame *MainWindowLayout::qrPreviewFrame()
{
    return &mQrPreviewFrame;
}

QLineEdit *MainWindowLayout::fileDirectoryField()
{
    return &mFileDirectoryField;
}

QLineEdit *MainWindowLayout::fileNameField()
{
    return &mFileNameField;
}

QPushButton *MainWindowLayout::fileDirectoryOpenButton()
{
    return &mFileDirectoryOpenButton;
}

QPushButton *MainWindowLayout::fileDirectoryBrowseButton()
{
    return &mFileDirectoryBrowseButton;
}

QComboBox *MainWindowLayout::fileExtensionMenu()
{
    return &mFileExtensionMenu;
}

QTextEdit *MainWindowLayout::contentField()
{
    return &mContentField;
}

QComboBox *MainWindowLayout::errorCorrectionMenu()
{
    return &mErrorCorrectionMenu;
}

QPushButton *MainWindowLayout::foregroundColorButton()
{
    return &mForegroundColorButton;
}
    
QPushButton *MainWindowLayout::backgroundColorButton()
{
    return &mBackgroundColorButton;
}

QPushButton *MainWindowLayout::resetColorsButton()
{
    return &mResetColorsButton;
}

QSlider *MainWindowLayout::borderSizeSlider()
{
    return &mBorderSizeSlider;
}

QLabel *MainWindowLayout::borderSizeValueLabel()
{
    return &mBorderSizeValueLabel;
}

QSlider *MainWindowLayout::imageSizeSlider()
{
    return &mImageSizeSlider;
}

QLabel *MainWindowLayout::imageSizeValueLabel()
{
    return &mImageSizeValueLabel;
}

QFrame *MainWindowLayout::logoFrame()
{
    return &mLogoFrame;
}

QLineEdit *MainWindowLayout::logoPathField()
{
    return &mLogoPathField;
}

QLabel *MainWindowLayout::logoSizeLabel()
{
    return &mLogoSizeLabel;
}

QLabel *MainWindowLayout::logoFileLabel()
{ 
    return &mLogoFileLabel;
}

QLabel *MainWindowLayout::logoSizeValueLabel()
{
    return &mLogoSizeValueLabel;
}

QCheckBox *MainWindowLayout::logoEnabledCheckBox()
{
    return &mLogoEnabledCheckBox;
}

QSlider *MainWindowLayout::logoSizeSlider()
{
    return &mLogoSizeSlider;
}

QPushButton *MainWindowLayout::changeLogoButton()
{
    return &mChangeLogoButton;
}

QPushButton *MainWindowLayout::saveButton()
{
    return &mSaveButton;
}

QCheckBox *MainWindowLayout::previewCheckBox()
{
    return &mPreviewCheckBox;
}

QCheckBox *MainWindowLayout::openAfterExportCheckBox() 
{
    return &mOpenAfterExportCheckBox;
}

void MainWindowLayout::addOutputFileDirectoryWidgets()
{
    mControlLayout.addWidget(&mFileDirectoryLabel);

    mFileDirectoryLabel.setText(tr("Output file directory"));
    mFileDirectoryField.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    mFileDirectoryField.setReadOnly(true);
    mFileDirectoryBrowseButton.setMaximumWidth(100);
    mFileDirectoryBrowseButton.setText(tr("Browse..."));
    mFileDirectoryOpenButton.setMaximumWidth(mFileDirectoryBrowseButton.maximumWidth());
    mFileDirectoryOpenButton.setText(tr("Show"));

    mFileDirectoryLayout.addWidget(&mFileDirectoryField);
    mFileDirectoryLayout.addWidget(&mFileDirectoryOpenButton);
    mFileDirectoryLayout.addWidget(&mFileDirectoryBrowseButton);

    mControlLayout.addLayout(&mFileDirectoryLayout);
}

void MainWindowLayout::addOutputFileNameWidgets()
{
    mFileNameLabel.setText(tr("Output file name and format"));
    mFileNameField.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    mFileExtensionMenu.setMinimumWidth(100);

    mControlLayout.addWidget(&mFileNameLabel);

    mFileNameLayout.addWidget(&mFileNameField);
    mFileNameLayout.addWidget(&mFileExtensionMenu);

    mControlLayout.addLayout(&mFileNameLayout);
}

void MainWindowLayout::addContentWidgets()
{
    mContentFieldLabel.setText(tr("Content"));

    mControlLayout.addWidget(&mContentFieldLabel);
    mControlLayout.addWidget(&mContentField);
}

void MainWindowLayout::addAdvancedOptionsWidgets()
{
    mOptionsGroupBox.setTitle(tr("Advanced options"));

    mBackgroundColorButton.setIcon(paletteIcon);
    mBackgroundColorButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    mBackgroundColorButton.setMaximumWidth(200);

    mErrorCorrectionLabel.setText(tr("Error correction level"));
    mForegroundColorLabel.setText(tr("Primary color"));
    mForegroundColorLabel.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    mBackgroundColorLabel.setText(tr("Background color"));
    mBackgroundColorLabel.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    mForegroundColorButton.setIcon(paletteIcon);
    mForegroundColorButton.setSizePolicy(mBackgroundColorButton.sizePolicy());
    mForegroundColorButton.setMaximumWidth(mBackgroundColorButton.maximumWidth());
    mErrorCorrectionMenu.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    mResetColorsButton.setMaximumWidth(200);
    mResetColorsButton.setText(tr("Reset colors"));

    mColorPickers.setAlignment(Qt::AlignLeft);

    mOptionsLayout.addWidget(&mErrorCorrectionLabel);
    mOptionsLayout.addWidget(&mErrorCorrectionMenu);

    mColorPickers.addWidget(&mForegroundColorLabel);
    mColorPickers.addWidget(&mForegroundColorButton);
    mColorPickers.addWidget(&mBackgroundColorLabel);
    mColorPickers.addWidget(&mBackgroundColorButton);

    mOptionsLayout.addLayout(&mColorPickers);
    mOptionsLayout.addWidget(&mResetColorsButton);

    mBorderSizeLabel.setText(tr("Border size"));
    mBorderSizeSlider.setMaximumWidth(300);
    mBorderSizeLayout.setAlignment(Qt::AlignLeft);
    mImageSizeLabel.setText(tr("Image size"));
    mImageSizeSlider.setMaximumWidth(mBorderSizeSlider.maximumWidth());
    mImageSizeValueLabel.setText("0");
    mBorderSizeValueLabel.setText("0");

    mOptionsSliders.addWidget(&mBorderSizeLabel);
    mBorderSizeLayout.addWidget(&mBorderSizeSlider);
    mBorderSizeLayout.addWidget(&mBorderSizeValueLabel);

    mOptionsSliders.addLayout(&mBorderSizeLayout);
    mOptionsSliders.addWidget(&mImageSizeLabel);

    mImageSizeLayout.setAlignment(Qt::AlignLeft);

    mImageSizeLayout.setContentsMargins(QMargins(0, 0, 0, 10));
    mImageSizeLayout.addWidget(&mImageSizeSlider);
    mImageSizeLayout.addWidget(&mImageSizeValueLabel);

    mOptionsSliders.addLayout(&mImageSizeLayout);
    mOptionsLayout.addLayout(&mOptionsSliders);

    addLogoOptionsWidgets();

    mOptionsGroupBox.setLayout(&mOptionsLayout);
    mControlLayout.addWidget(&mOptionsGroupBox);
}

void MainWindowLayout::addLogoOptionsWidgets()
{
    mOptionsLayout.addWidget(&mLogoEnabledCheckBox);

    mLogoFrame.setFrameStyle(QFrame::StyledPanel);

    mLogoLayout.setAlignment(Qt::AlignLeft);

    mLogoEnabledCheckBox.setText(tr("Enable logo (disables SVG format)"));
    mLogoEnabledCheckBox.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mLogoSizeLabel.setText(tr("Logo size"));
    mChangeLogoButton.setText(tr("Load image"));
    mLogoFileLabel.setText(tr("Logo file"));
    mLogoSizeSlider.setMaximumWidth(mImageSizeSlider.maximumWidth());
    mLogoSizeSlider.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mChangeLogoButton.setMaximumWidth(150);
    mChangeLogoButton.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    mLogoPathField.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mLogoPathField.setReadOnly(true);
    mLogoSizeValueLabel.setText("0");
    mLogoSizeValueLabel.setMaximumWidth(50);
    mLogoSizeValueLabel.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    mLogoLayout.addWidget(&mLogoSizeLabel);

    mLogoSizeLayout.setAlignment(Qt::AlignLeft);
    mLogoSizeLayout.addWidget(&mLogoSizeSlider);
    mLogoSizeLayout.addWidget(&mLogoSizeValueLabel);

    mLogoFileLayout.addWidget(&mLogoPathField);
    mLogoFileLayout.addWidget(&mChangeLogoButton);

    mLogoLayout.addLayout(&mLogoSizeLayout);
    mLogoLayout.addWidget(&mLogoFileLabel);
    mLogoLayout.addLayout(&mLogoFileLayout);
    mLogoFrame.setLayout(&mLogoLayout);

    mOptionsLayout.addWidget(&mLogoFrame);
}

void MainWindowLayout::addCheckBoxes()
{
    mPreviewCheckBox.setText(tr("Enable preview"));
    mPreviewCheckBox.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    mOpenAfterExportCheckBox.setText(tr("Show image after export"));
    mOpenAfterExportCheckBox.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    mCheckBoxesLayout.setAlignment(Qt::AlignLeft);

    mCheckBoxesLayout.addWidget(&mPreviewCheckBox);
    mCheckBoxesLayout.addWidget(&mOpenAfterExportCheckBox);

    mControlLayout.addLayout(&mCheckBoxesLayout);
}

void MainWindowLayout::addFooter()
{
    mFooterLayout.setContentsMargins(QMargins(0, 8, 0, 0));
    mFooterLayout.setAlignment(Qt::AlignHCenter);

    mSaveButton.setText(tr("Save as file"));
    mSaveButton.setMaximumWidth(400);
    mSaveButton.setMinimumHeight(40);
    mSaveButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    mFooterLayout.addWidget(&mSaveButton);
    mControlLayout.addLayout(&mFooterLayout);
}