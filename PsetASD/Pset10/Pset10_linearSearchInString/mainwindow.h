#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();


private slots:
    void on_pbStart_clicked();



    void on_pbRandomize_clicked();

private:
    bool compareTwoSubstring(const std::string& text, size_t firstBegin,size_t firstEnd,size_t secondBegin, size_t secondEnd,std::string& output) const;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
