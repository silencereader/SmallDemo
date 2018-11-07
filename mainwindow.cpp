#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <audioclient.h>
#include <Windows.h>

#pragma comment (lib, "ole32.lib")//很关键的一行代码//避免CoCreateInstance报2019错误

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalSlider->hide();
    ui->label_2->hide();
    //取系统音量并转换所需类型
    int m_volume = GetSysVolume();
    QString text = QString::number(m_volume);
    //设置滑条与显示数值同步
    ui->label_2->setText(text);
    ui->horizontalSlider->setValue(m_volume);
//    connect(Timer,SIGNAL(timeout()),ui->horizontalSlider,SLOT(Slot_Release()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    QString volumeNum = QString::number(position);
    ui->pushButton->setText(volumeNum);
//    ui->label->setText(volumeNum);
    int volume =ui->horizontalSlider->value();
    QString StrVolume = QString::number(volume);
    ui->label_2->setText(StrVolume);
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    int volume =ui->horizontalSlider->value();
    QString StrVolume = QString::number(volume);
    ui->label_2->setText(StrVolume);
//    VolumeArgs VolumeValue;
//    VolumeValue.bScalar = false;
//    VolumeValue.nVolume = (double)volume;
//    ChangeSysVolume(VolumeValue);
    SetVolum(volume);
    QString VolumeNum = QString::number(GetSysVolume());
    ui->label->setText(VolumeNum);
}

void MainWindow::on_pushButton_clicked()
{
    ui->horizontalSlider->show();
    ui->label_2->show();
}

void MainWindow::Slot_Release()
{
    ui->horizontalSlider->hide();
    ui->label->setStyleSheet("background-color: rgb(255, 255, 0);");
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    m_VolumeValue = value;
    QString StrVolume = QString::number(value);
    ui->label_2->setText(StrVolume);
}

int MainWindow::GetSysVolume()
{
    //获取系统音量并量化为0-100
    HRESULT hr;
    IMMDeviceEnumerator* pDeviceEnumerator=0;
    IMMDevice* pDevice=0;
    IAudioEndpointVolume* pAudioEndpointVolume=0;
    IAudioClient* pAudioClient=0;

    float fVolume = 0;

    try{
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),NULL,CLSCTX_ALL,__uuidof(IMMDeviceEnumerator),(void**)&pDeviceEnumerator);
    if(FAILED(hr)) throw "CoCreateInstance";
    hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender,eMultimedia,&pDevice);
    if(FAILED(hr)) throw "GetDefaultAudioEndpoint";
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void**)&pAudioEndpointVolume);
    if(FAILED(hr)) throw "pDevice->Active";
    hr = pDevice->Activate(__uuidof(IAudioClient),CLSCTX_ALL,NULL,(void**)&pAudioClient);
    if(FAILED(hr)) throw "pDevice->Active";

    hr = pAudioEndpointVolume->GetMasterVolumeLevelScalar(&fVolume);
    if(FAILED(hr)) throw "GetMasterVolumeLevel";
//    qDebug()<<"fVolume:"<<fVolume<<endl;

    pAudioClient->Release();
    pAudioEndpointVolume->Release();
    pDevice->Release();
    pDeviceEnumerator->Release();
    }
    catch(...){
    if(pAudioClient) pAudioClient->Release();
    if(pAudioEndpointVolume) pAudioEndpointVolume->Release();
    if(pDevice) pDevice->Release();
    if(pDeviceEnumerator) pDeviceEnumerator->Release();
    throw;
    }

    return fVolume*100;

}

//void MainWindow::ChangeSysVolume(VolumeArgs volumeargs)
//{
//    //调整音量
//        //nVolume 为0.0-0.1之间
//        HRESULT hr = NULL;//HRESULT一种数据类型
//        CoInitialize(NULL);
//        IMMDeviceEnumerator *deviceEnumerator = NULL;
//        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
//                              __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
//        IMMDevice *defaultDevice = NULL;

//        hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
//        deviceEnumerator->Release();
//        deviceEnumerator = NULL;

//        IAudioEndpointVolume *endpointVolume = NULL;
//        hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
//                                     CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
//        defaultDevice->Release();
//        defaultDevice = NULL;

//        BOOL IsMute;//是否静音
//        endpointVolume->GetMute(&IsMute);
//        if (IsMute)
//        {
//            IsMute = FALSE;//取消静音
//            endpointVolume->SetMute(IsMute, NULL);
//        }

//        float currentVolume = 0;
//        endpointVolume->GetMasterVolumeLevel(&currentVolume);//单位为dB
//        hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);//0.0-0.1之间

//        if (volumeargs.bScalar == false)
//        {
//            hr = endpointVolume->SetMasterVolumeLevel((float)volumeargs.nVolume, NULL);
//        }
//        else if (volumeargs.bScalar == true)
//        {
//            hr = endpointVolume->SetMasterVolumeLevelScalar((float)volumeargs.nVolume, NULL);
//        }
//        endpointVolume->Release();

//        CoUninitialize();
//}

bool MainWindow::SetVolum(int volume)
{
    HRESULT hr;
    IMMDeviceEnumerator* pDeviceEnumerator=0;
    IMMDevice* pDevice=0;
    IAudioEndpointVolume* pAudioEndpointVolume=0;
    IAudioClient* pAudioClient=0;

    try{
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),NULL,CLSCTX_ALL,__uuidof(IMMDeviceEnumerator),(void**)&pDeviceEnumerator);
        if(FAILED(hr)) throw "CoCreateInstance";
        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender,eMultimedia,&pDevice);
        if(FAILED(hr)) throw "GetDefaultAudioEndpoint";
        hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void**)&pAudioEndpointVolume);
        if(FAILED(hr)) throw "pDevice->Active";
        hr = pDevice->Activate(__uuidof(IAudioClient),CLSCTX_ALL,NULL,(void**)&pAudioClient);
        if(FAILED(hr)) throw "pDevice->Active";


        float fVolume;
        int level = volume;
        fVolume = level/100.0f;
        hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(fVolume,&GUID_NULL);
        if(FAILED(hr)) throw "SetMasterVolumeLevelScalar";

        pAudioClient->Release();
        pAudioEndpointVolume->Release();
        pDevice->Release();
        pDeviceEnumerator->Release();
        return true;
    }

    catch(...){
        if(pAudioClient) pAudioClient->Release();
        if(pAudioEndpointVolume) pAudioEndpointVolume->Release();
        if(pDevice) pDevice->Release();
        if(pDeviceEnumerator) pDeviceEnumerator->Release();
        throw;
    }
    return false;
}
