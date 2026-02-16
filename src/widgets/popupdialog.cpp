#include "popupdialog.hpp"
#include <QShowEvent>
#include <QApplication>

using namespace QrCodeGeneratorGui;

PopupDialog::PopupDialog(QWidget *parent, QString text, bool dialog, Type type) : QDialog(parent, Qt::Dialog)
{
    setWindowModality(Qt::WindowModality::ApplicationModal);

    mTextLabel.setText(text);
    mTextLabel.setAlignment(Qt::AlignHCenter);

    mTextLabel.setAlignment(Qt::AlignCenter);
    mIconLabel.setAlignment(Qt::AlignCenter);

    switch (type) {
        case SUCCESS:
            mIcon = QIcon(":/images/ok.svg");
            setWindowTitle(tr("Success"));
            break;
        case WARNING:
            mIcon = QIcon(":/images/warning.svg");
            setWindowTitle(tr("Warning!"));
            break;
        case ERROR:
            mIcon = QIcon(":/images/error.svg");
            setWindowTitle(tr("An error occurred"));
            break;
    }

    auto iconPixmap = mIcon.pixmap(QSize(28, 28));
    mIconLabel.setPixmap(iconPixmap);

    mMessageLayout.addWidget(&mIconLabel);
    mMessageLayout.addWidget(&mTextLabel);
    mLayout.addLayout(&mMessageLayout);
    mLayout.setSpacing(12);
    mLayout.setContentsMargins(QMargins(16, 16, 16, 16));
    mMessageLayout.setContentsMargins(QMargins(0, 0, 0, 8));

    mButtonLayout.addWidget(&mAcceptButton);  
    
    mAcceptButton.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    mRejectButton.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    mAcceptButton.setMaximumWidth(80);
    mRejectButton.setMaximumWidth(80);
    mButtonLayout.setSizeConstraints(QLayout::SetMinimumSize, QLayout::SetMinimumSize);

    if (dialog) {
        mAcceptButton.setText(tr("Yes"));
        mRejectButton.setText(tr("No"));

        mButtonLayout.addWidget(&mRejectButton);

        connect(&mAcceptButton, &QPushButton::clicked, this, &PopupDialog::accept);
        connect(&mRejectButton, &QPushButton::clicked, this, &PopupDialog::reject);
    }
    else {
        mAcceptButton.setText(tr("OK"));
        connect(&mAcceptButton, &QPushButton::clicked, this, &PopupDialog::hide);
    }

    mLayout.addLayout(&mButtonLayout);

    setLayout(&mLayout);
}

void PopupDialog::showEvent(QShowEvent *event)
{
    if (parent()->isWidgetType()) {
        QWidget *parentWidget = dynamic_cast<QWidget *>(parent());
        QRect parentGeometry = parentWidget->geometry();

        move(
            parentGeometry.x() + parentGeometry.width() / 2 - geometry().width() / 2, 
            parentGeometry.y() + parentGeometry.height() / 2 - geometry().height() / 2);
    }

    setFixedSize(minimumSize());
}