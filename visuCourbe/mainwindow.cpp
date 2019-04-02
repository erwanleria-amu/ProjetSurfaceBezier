/* R. Raffin
 * M1 Informatique, Aix-Marseille Université
 * Fenêtre principale
 * Au cas où, l'UI contient une barre de menu, une barre de status, une barre d'outils (cf QMainWindow).
 * Une zone est laissée libre à droite du Widget OpenGL pour mettre de futurs contrôles ou informations.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myopenglwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_2->setNum(ui->openGLWidget->getNbCol());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->label_2->setNum(ui->openGLWidget->getNbCol()-1);
    ui->openGLWidget->setNbCol(ui->label_2->text().toInt());
    ui->openGLWidget->setSizeChanged();
    qDebug() << ui->openGLWidget->getNbCol() << endl;
    ui->openGLWidget->upd();
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->label_2->setNum(ui->openGLWidget->getNbCol()+1);
    ui->openGLWidget->setNbCol(ui->label_2->text().toInt());
    ui->openGLWidget->setSizeChanged();
    qDebug() << ui->openGLWidget->getNbCol() << endl;
    ui->openGLWidget->upd();
}

void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
    ui->openGLWidget->setU((float) (1/ui->openGLWidget->curDiscreteObj->getStep()) * position/100 * ui->openGLWidget->curDiscreteObj->getStep());
    ui->openGLWidget->upd();
}



void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    ui->openGLWidget->setV((float) (1/ui->openGLWidget->curDiscreteObj->getStep()) * position/100 * ui->openGLWidget->curDiscreteObj->getStep());
    ui->openGLWidget->upd();
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    if(arg1 > 1)
        arg1 = 1;

    ui->openGLWidget->setStep(arg1);
    ui->openGLWidget->upd();
}
