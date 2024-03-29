#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "myopenglwidget.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_sliderMoved(int position);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pushButton_3_clicked();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
