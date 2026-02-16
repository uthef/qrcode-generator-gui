#ifndef MAINWINDOWLAYOUT_HPP
#define MAINWINDOWLAYOUT_HPP

#include <QMainWindow>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QGroupBox>
#include <QScrollArea>

namespace QrCodeGeneratorGui
{

class MainWindowLayout : public QObject
{
    Q_OBJECT
public:
    MainWindowLayout(QMainWindow *window);
    QMainWindow *window() const;

    QLabel *qrPreviewImage();
    QFrame *qrPreviewFrame();

    QLineEdit *fileDirectoryField();
    QLineEdit *fileNameField();
    QPushButton *fileDirectoryOpenButton();
    QPushButton *fileDirectoryBrowseButton();
    QComboBox *fileExtensionMenu();

    QTextEdit *contentField();

    QComboBox *errorCorrectionMenu();

    QPushButton *foregroundColorButton();
    QPushButton *backgroundColorButton();
    QPushButton *resetColorsButton();

    QSlider *borderSizeSlider();
    QLabel *borderSizeValueLabel();

    QSlider *imageSizeSlider();
    QLabel *imageSizeValueLabel();

    QFrame *logoFrame();
    QLineEdit *logoPathField();
    QLabel *logoSizeLabel();
    QLabel *logoFileLabel();
    QLabel *logoSizeValueLabel();
    QSlider *logoSizeSlider();
    QPushButton *changeLogoButton();
    QCheckBox *logoEnabledCheckBox(); 

    QPushButton *saveButton();
    QCheckBox *previewCheckBox();
    QCheckBox *openAfterExportCheckBox();
private:
    QIcon paletteIcon;

    QMainWindow *mWindow = nullptr;
    QWidget mCentralWidget;
    QBoxLayout mLayout;
    QVBoxLayout mControlLayout;
    QFrame mQrPreviewFrame;
    QVBoxLayout mQrPreviewLayout;
    QLabel mQrPreviewImage;
    
    QLabel mFileDirectoryLabel;
    QHBoxLayout mFileDirectoryLayout;
    QLineEdit mFileDirectoryField;
    QPushButton mFileDirectoryOpenButton;
    QPushButton mFileDirectoryBrowseButton;

    QLabel mFileNameLabel;
    QHBoxLayout mFileNameLayout;
    QLineEdit mFileNameField;
    QComboBox mFileExtensionMenu;

    QLabel mContentFieldLabel;
    QTextEdit mContentField;

    QWidget mOptionsLayoutContainer;
    QGroupBox mOptionsGroupBox;
    QScrollArea mOptionsScrollArea;
    QVBoxLayout mOptionsLayout;
    QHBoxLayout mColorPickers;
    QVBoxLayout mOptionsSliders;
    QVBoxLayout mOptionsGroupBoxLayout;

    QLabel mErrorCorrectionLabel;
    QComboBox mErrorCorrectionMenu;
    QLabel mForegroundColorLabel;
    QPushButton mForegroundColorButton;
    QLabel mBackgroundColorLabel;
    QPushButton mBackgroundColorButton;
    QPushButton mResetColorsButton;

    QLabel mBorderSizeLabel;
    QLabel mBorderSizeValueLabel;
    QHBoxLayout mBorderSizeLayout;
    QSlider mBorderSizeSlider;
    QLabel mImageSizeLabel;
    QLabel mImageSizeValueLabel;
    QHBoxLayout mImageSizeLayout;
    QSlider mImageSizeSlider;

    QFrame mLogoFrame;
    QVBoxLayout mLogoLayout;
    QHBoxLayout mLogoSizeLayout;
    QHBoxLayout mLogoFileLayout;
    QLabel mLogoSizeLabel;
    QLabel mLogoFileLabel;
    QCheckBox mLogoEnabledCheckBox;
    QSlider mLogoSizeSlider;
    QLabel mLogoSizeValueLabel;
    QLineEdit mLogoPathField;
    QPushButton mChangeLogoButton;

    QVBoxLayout mFooterLayout;
    QHBoxLayout mCheckBoxesLayout;
    QCheckBox mOpenAfterExportCheckBox;
    QCheckBox mPreviewCheckBox;
    QPushButton mSaveButton;

    void addOutputFileDirectoryWidgets();
    void addOutputFileNameWidgets();
    void addContentWidgets();
    void addAdvancedOptionsWidgets();
    void addLogoOptionsWidgets();
    void addCheckBoxes();
    void addFooter();
};

}

#endif