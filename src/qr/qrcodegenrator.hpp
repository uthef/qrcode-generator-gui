#ifndef QRCODEGENERATOR_HPP
#define QRCODEGENERATOR_HPP

#include <string>
#include <nayuki/qr-code-generator/qrcodegen.hpp>
#include <QPainter>

namespace QrCodeGeneratorGui
{

class QrCodeGenerator
{
public:
    static QByteArray encodeTextAsSvgString(
        const char *text, 
        qrcodegen::QrCode::Ecc errorCorrection, 
        QColor foreground, 
        QColor background, 
        float border = 0);

    static void encodeTextAsImage(
        const char *text, 
        qrcodegen::QrCode::Ecc errorCorrection, 
        QPainter &painter, 
        QColor foreground, 
        QColor background, 
        float border,
        float logoSize = 0.0f,
        QImage *logo = nullptr);
    
    static qrcodegen::QrCode::Ecc mapEcc(QString value);
private:
    static void validateBorderSizeValue(float border);
};

}

#endif