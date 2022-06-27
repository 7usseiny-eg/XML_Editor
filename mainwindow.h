#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include "convert.h"
#include "Compress.h"
#include "consistency.h"
#include "formatter.h"

#define HIDDEN false
#define SHOWN true

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionSave_as_triggered();

    void on_actionOpen_2_triggered();

    void on_actionCurrent_File_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_actionCompress_triggered();

    void on_actionDecompress_triggered();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_actionFormat_triggered();

    void on_pushButton_9_clicked();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    int darkMode = 0;
    bool checked = false;
    bool button1 = SHOWN;
    bool button2 = SHOWN;
};
#endif // MAINWINDOW_H
