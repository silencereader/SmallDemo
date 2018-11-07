#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

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
    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderReleased();

    void on_pushButton_clicked();

    void Slot_Release();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    int time;
    QTimer *Timer;
    int m_VolumeValue;
    struct VolumeArgs
    {
        double nVolume;
        bool bScalar;
    };

private:
    int GetSysVolume();
//    void ChangeSysVolume(VolumeArgs);
    bool SetVolum(int);
};

#endif // MAINWINDOW_H
