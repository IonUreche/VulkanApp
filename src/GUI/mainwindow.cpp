#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	m_window = new Window(width(), height());
	m_windowContainer = QWidget::createWindowContainer(m_window);
	ui->verticalLayout->addWidget(m_windowContainer);
}

MainWindow::~MainWindow()
{
    delete ui;
}
