#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	vulkan_window = new VulkanWindow(width(), height());
	window_container = QWidget::createWindowContainer(vulkan_window);
	ui->verticalLayout->addWidget(window_container);
}

MainWindow::~MainWindow()
{
    delete ui;
}
