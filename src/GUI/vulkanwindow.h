/*
 * A reimplemented QWindow that is displayed by the Qt GUI. This is where everything related to rendering happens.
 * */

#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H
#include <QWindow>

class VulkanWindow : public QWindow
{
    Q_OBJECT
public:
    VulkanWindow();
    ~VulkanWindow();
};

#endif // VULKANWINDOW_H
