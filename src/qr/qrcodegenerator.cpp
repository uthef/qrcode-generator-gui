#include "qrcodegenrator.hpp"

#include <iostream>

using namespace QrCodeGeneratorGui;
using namespace qrcodegen;

QByteArray QrCodeGenerator::encodeTextAsSvgString(
	const char *text, 
	QrCode::Ecc errorCorrection, 
	QColor foreground, 
	QColor background, 
	float border)
{
	validateBorderSizeValue(border);

    auto qrCode = QrCode::encodeText(text, errorCorrection);
	
	QString svg;
	float relativeBorder = qrCode.getSize() / 100.0f * border;

	svg.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	svg.append("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
	svg.append("<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ");

	svg.append(
        QString("%1 %2\" stroke=\"none\">\n")
            .arg(qrCode.getSize() + relativeBorder * 2) 
            .arg(qrCode.getSize() + relativeBorder * 2)
    );

	QString foregroundColor = QString("rgba(%1, %2, %3, %4)")
		.arg(foreground.red())
		.arg(foreground.green())
		.arg(foreground.blue())
		.arg(foreground.alphaF());

	QString backgroundColor = QString("rgba(%1, %2, %3, %4)")
		.arg(background.red())
		.arg(background.green())
		.arg(background.blue())
		.arg(background.alphaF());

	svg.append(QString("\t<rect width=\"100%\" height=\"100%\" fill=\"%1\"/>\n").arg(backgroundColor));
	svg.append("\t<path d=\"");

	for (int y = 0; y < qrCode.getSize(); y++) {
		for (int x = 0; x < qrCode.getSize(); x++) {
			if (qrCode.getModule(x, y)) {
				if (x != 0 || y != 0)
					svg.append(" ");

                svg.append(
                    QString("M%1,%2h1v1h-1z").arg(x + relativeBorder).arg(y + relativeBorder)
                );
			}
		}
	}

	svg.append(QString("\" fill=\"%1\"/>\n").arg(foregroundColor));
	svg.append("</svg>\n");

	return svg.toUtf8();
}

void QrCodeGenerator::encodeTextAsImage(
	const char *text, 
	QrCode::Ecc errorCorrection, 
	QPainter &painter, 
	QColor foreground, 
	QColor background, 
	float border, 
	float logoSize,
	QImage *logo)
{
	validateBorderSizeValue(border);

	auto qrCode = QrCode::encodeText(text, errorCorrection);

	float width = (float)painter.window().width();
	float relativeBorder = width / 100.0f / qrCode.getSize() * border;
	float cellSize = width / qrCode.getSize() - relativeBorder;
	float topLeftMargin = relativeBorder / 2 * qrCode.getSize();
	
	auto foregroundColor = foreground;
	auto backgroundColor = background;

	painter.fillRect(QRectF(0, 0, width, painter.window().height()), backgroundColor);

	for (int y = 0; y < qrCode.getSize(); y++) {
		for (int x = 0; x < qrCode.getSize(); x++) {
			if (qrCode.getModule(x, y)) {
				auto rect = QRectF(topLeftMargin + x * cellSize, topLeftMargin + y * cellSize, cellSize, cellSize);
				painter.fillRect(rect, foregroundColor);
			}
		}
	}

	if (!logo || logoSize == 0) 
		return;

	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	float logoRatio = (float)logo->height() / (float)logo->width();
	float realLogoSizeX = (painter.window().width() - topLeftMargin * 2) * logoSize / 100.0f;
	float realLogoSizeY = realLogoSizeX * logoRatio;
	auto logoRect = QRectF(width / 2 - realLogoSizeX / 2, width / 2 - realLogoSizeY / 2, realLogoSizeX, realLogoSizeY);

	painter.drawImage(logoRect, *logo);
}

QrCode::Ecc QrCodeGenerator::mapEcc(QString value)
{
	if (value == "L (7%)" || value == "L")
		return QrCode::Ecc::LOW;

	if (value == "M (15%)" || value == "M")
		return QrCode::Ecc::MEDIUM;
	
	if (value == "Q (25%)" || value == "Q")
		return QrCode::Ecc::QUARTILE;
	
	if (value == "H (30%)" || value == "H")
		return QrCode::Ecc::HIGH;
	
	return QrCode::Ecc::LOW;
}

void QrCodeGenerator::validateBorderSizeValue(float border)
{
    if (border < 0)
		throw std::domain_error("Border must be non-negative");

	if (border > 100)
		throw std::overflow_error("Border too large");
}