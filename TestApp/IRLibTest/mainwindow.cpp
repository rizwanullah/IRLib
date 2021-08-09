#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../IRLib/Encoder.h"
#include "../../IRLib/Decoder.h"
#include "../../IRLib/RemoteFinder.h"
#include "../../IRLib/json.hpp"

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    this->statusBar()->setSizeGripEnabled(false);


}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string MainWindow::printHexStringToBinaryString(std::string hexStr, bool printByteNum) {

    quint32 buffLen = hexStr.size()/2;
    quint8 buff[buffLen];
    quint16 val;
    for (quint32 i = 0; i < (buffLen * 2); i += 2) {
        std::string token = hexStr.substr(i, 2);
        std::stringstream sstream(token);
        sstream >> std::hex >> val;
        buff[i/2] = (quint8) val;
    }

    std::stringstream sstream;
    for (quint32 i = 0; i < buffLen; i++) {
        sstream << " ";
        if (printByteNum) {
            sstream << i;
            sstream << "-";
        }
        uint8_t b = buff[i];
        for (quint8 j = 0; j < 8; j++) {
            if ((b & 0x80) == 0x80) // 1
                sstream << "1";
            else // 0
                sstream << "0";
            b <<= 1;
        }
    }
    return sstream.str();
}

void MainWindow::on_decDecodePushButton_clicked()
{
    QString irCodeStr = ui->decIRCodeTextEdit->toPlainText();
    if (!irCodeStr.isEmpty()) {
        qint32 rmtId = 0;
        if (ui->decChigo1RadioButton->isChecked())
            rmtId = 1000;
        else if (ui->decAux1RadioButton->isChecked())
            rmtId = 1001;
        else if (ui->decTcl1RadioButton->isChecked())
            rmtId = 1002;
        else if (ui->decChigo2RadioButton->isChecked())
            rmtId = 1003;

        json encodedIrJson;
        encodedIrJson["remoteId"] = rmtId;
        encodedIrJson["encodedIrData"] = irCodeStr.toUtf8().constData();

        Decoder *decoder = new Decoder();
        std::string decodedIrJsonStr = decoder->decode(encodedIrJson.dump());
        json decodedIrJson = json::parse(decodedIrJsonStr);
        std::string decodedIrPacketHexStr = decodedIrJson["decodedIrPacket"];
        std::string decodedIrPacketBinStr = printHexStringToBinaryString(decodedIrPacketHexStr, true);

        ui->decIRPacketTextEdit->setText(QString::fromStdString(decodedIrPacketBinStr));
        ui->decIRJsonTextEdit->setText(QString::fromStdString(decodedIrJsonStr));
    } else {
        QMessageBox::warning(this, "Warning", "Please enter IR Code!");
    }
}

void MainWindow::on_frFindPushButton_clicked()
{
    QString barcodeStr = ui->frBarcodeLineEdit->text();
    if (!barcodeStr.isEmpty()) {
        RemoteFinder *remoteFinder = new RemoteFinder();
        qint32 remoteId = remoteFinder->findRemoteId(barcodeStr.toUpper().toUtf8().constData());
        qDebug() << "remoteId: " << remoteId;
        if (remoteId != RF_REMOTE_ID_NOT_FOUND) {
            ui->frFoundIdLabel->setText(QString::number(remoteId));
        } else {
            ui->frFoundIdLabel->setText("None");
        }
    } else {
        QMessageBox::warning(this, "Warning", "Please enter barcode!");
    }
}
