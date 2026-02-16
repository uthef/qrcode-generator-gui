#ifndef POPUPDIALOG_HPP
#define POPUPDIALOG_HPP

#include "QDialog"
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>

namespace QrCodeGeneratorGui
{

class PopupDialog : public QDialog
{
    Q_OBJECT
public:
    enum Type 
    {   
        SUCCESS,
        WARNING,
        ERROR
    };

    PopupDialog(QWidget *parent, QString text, bool dialog, Type type = SUCCESS);
private:
    QIcon mIcon;

    QVBoxLayout mLayout;
    QHBoxLayout mMessageLayout;
    QHBoxLayout mButtonLayout;
    QLabel mIconLabel;
    QLabel mTextLabel;
    QPushButton mAcceptButton;
    QPushButton mRejectButton;

    void showEvent(QShowEvent *event) override;
};

}

#endif