#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);
    ui->textEdit_2->setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
    checked = false;
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}


void MainWindow::on_actionSave_as_triggered()
{
    QString text = ui->textEdit->toPlainText();
    if(text == ""){
        QMessageBox::warning(this, "Error", "You have typed nothing to save");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    out << text;
    file.close();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}



void MainWindow::on_actionOpen_2_triggered() // CONVERT OPEN
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    ui->textEdit_2->setText(QString::fromStdString(convert_json(Parse_XML(text.toStdString()), 1)));
    file.close();
}


void MainWindow::on_actionCurrent_File_triggered() // CONVERT
{
    if(currentFile == ""){
        if(darkMode == 0){
            ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> You have not opened a file");
        }
        return;
    }
    QFile file(currentFile);
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit_2->setText(QString::fromStdString(convert_json(Parse_XML(text.toStdString()), 1)));
    file.close();
}


void MainWindow::on_pushButton_2_clicked() //OPEN FILE
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}


void MainWindow::on_pushButton_3_clicked() //COMPRESS
{
    if(currentFile == ""){
        if(darkMode == 0){
            ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> You have not opened a file");
        }
        return;
    }
    QByteArray array = currentFile.toLocal8Bit();
    const char *buffer = array.data();
    compress_data(buffer, "comp.dat");
    ui->textEdit_2->setTextBackgroundColor(QColor(255, 0, 0, 127));
    QFile file2("comp.dat");
    QString text = "Compressed File Name: ";
    text.append(file2.fileName());
    ui->textEdit_2->setText(text);
    ui->textEdit_2->setTextBackgroundColor(QColor(0,0,0,0));
}


void MainWindow::on_pushButton_clicked() //Convert
{
    if(currentFile == ""){
        if(darkMode == 0){
            ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> You have not opened a file");
        }
        return;
    }
    QFile file(currentFile);
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit_2->setText(QString::fromStdString(convert_json(Parse_XML(text.toStdString()), 1)));
    ui->pushButton_8->setText("Hide Converted Text Editor");
    file.close();
}


void MainWindow::on_pushButton_5_clicked() //DECOMPRESS
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    if(currentFile == ""){
        if(darkMode == 0){
            ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> You have not opened a file");
        }
        return;
    }
    QByteArray array = currentFile.toLocal8Bit();
    const char *buffer = array.data();
    decompress_data(buffer, "sample2.xml");
    ui->textEdit->setTextBackgroundColor(QColor(255, 0, 0, 127));
    ui->textEdit->setText(currentFile);
    ui->textEdit->setTextBackgroundColor(QColor(0,0,0,0));
    currentFile = "";
    file.close();
    QFile file2("sample2.xml");
    if(!file2.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    QTextStream in(&file2);
    QString text = in.readAll();
    ui->textEdit_2->setText(text);
}


void MainWindow::on_actionCompress_triggered() // COMPRESS
{
    if(currentFile == ""){
        if(darkMode == 0){
            ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> You have not opened a file");
        }
        return;
    }
    QByteArray array = currentFile.toLocal8Bit();
    const char *buffer = array.data();
    compress_data(buffer, "comp.dat");
    ui->textEdit_2->setTextBackgroundColor(QColor(255, 0, 0, 127));
    QFile file2("comp.dat");
    QString text = "Compressed File Name: ";
    text.append(file2.fileName());
    ui->textEdit_2->setText(text);
    ui->textEdit_2->setTextBackgroundColor(QColor(0,0,0,0));
    ui->pushButton_8->setText("Hide Compressed Text Editor");
}


void MainWindow::on_actionDecompress_triggered() // DECOMPRESS
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    if(currentFile == ""){
        if(darkMode == 0){
            ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> You have not opened a file");
        }
        return;
    }
    QByteArray array = currentFile.toLocal8Bit();
    const char *buffer = array.data();
    decompress_data(buffer, "sample2.xml");
    ui->textEdit->setTextBackgroundColor(QColor(255, 0, 0, 127));
    ui->textEdit->setText(currentFile);
    ui->textEdit->setTextBackgroundColor(QColor(0,0,0,0));
    currentFile = "";
    file.close();
    QFile file2("sample2.xml");
    if(!file2.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    QTextStream in(&file2);
    QString text = in.readAll();
    ui->textEdit_2->setText(text);
}


void MainWindow::on_pushButton_4_clicked() // CHECK
{
    if(currentFile == ""){
        if(darkMode == 0){
            ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> You have not opened a file");
        }
        return;
    }
    checked = true;
    bool read;
    string folder = currentFile.toLocal8Bit().constData();
    vector<string> str = fileReader(&read, folder);
    vector<QString> value = consistencyCheckCorrect(str);
    if(value[0].size() == 0)
        ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> There were no errors");
    else
        ui->textEdit_2->setText(value[0]);
    ui->textEdit->setText(value[1]);
        ui->pushButton_8->setText("Hide Checked Text Editor");
}


void MainWindow::on_pushButton_6_clicked() // CORRECT
{
    if(currentFile == ""){
        if(darkMode == 0){
            ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> You have not opened a file");
        }
        return;
    }
    checked = true;
    bool read;
    string folder = currentFile.toLocal8Bit().constData();
    vector<string> str = fileReader(&read, folder);
    vector<QString> value = consistencyCheckCorrect(str);
    QFile file("sample_corrected.xml");
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    if(value[0].size() == 0)
        ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> There were no errors to be corrected");
    else
        ui->textEdit_2->setText(text);
    ui->pushButton_8->setText("Hide Corrected Text Editor");
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    return;
}


void MainWindow::on_pushButton_7_clicked()
{
    if(button1 == SHOWN){
        button1 = HIDDEN;
        ui->textEdit->hide();
        ui->pushButton_7->setText("Show Text Editor");
    }
    else{
        button1 = SHOWN;
        ui->textEdit->show();
        ui->pushButton_7->setText("Hide Text Editor");
    }
}


void MainWindow::on_pushButton_8_clicked()
{
    if(button2 == SHOWN){
        button2 = HIDDEN;
        ui->textEdit_2->hide();
        ui->pushButton_8->setText("Show Program Text Editor");
    }
    else{
        button2 = SHOWN;
        ui->textEdit_2->show();
        ui->pushButton_8->setText("Hide Program Text Editor");
    }
}


void MainWindow::on_actionFormat_triggered()
{
    if(currentFile == ""){
        if(darkMode == 0){
            ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> You have not opened a file");
        }
        return;
    }
    QByteArray array = currentFile.toLocal8Bit();
    const char *buffer = array.data();
    QString text = QString::fromStdString(xml_formatter(buffer));
    ui->textEdit_2->setText(text);
    ui->pushButton_8->setText("Hide Formatted Text Editor");

}


void MainWindow::on_pushButton_9_clicked()
{
    if(currentFile == ""){
        if(darkMode == 0){
            ui->textEdit_2->setText("<font color=\"#0008F0\"> Error: <font color=\"#000000\"> You have not opened a file");
        }
        return;
    }
    QByteArray array = currentFile.toLocal8Bit();
    const char *buffer = array.data();
    QString text = QString::fromStdString(xml_formatter(buffer));
    ui->textEdit_2->setText(text);
    ui->pushButton_8->setText("Hide Formatted Text Editor");
}

