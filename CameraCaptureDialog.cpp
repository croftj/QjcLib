#include "CameraCaptureDialog.h"

#include <QCameraInfo>
#include <QImageWriter>

using namespace QcjLib;

CameraCaptureDialog::CameraCaptureDialog(QWidget *parent) : 
   QDialog(parent),
   m_hasImage(false),
   m_camera(nullptr)
{
   m_ui.setupUi(this);
   const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
   qDebug() << "cameras: " << cameras;
   foreach (const QCameraInfo &cam_info, cameras)
   {
      m_ui.cameraSelect->addItem(cam_info.description(), QVariant(cam_info.deviceName()));
   }
   const QCameraInfo camera_info = QCameraInfo::defaultCamera();
   qDebug() << "default camera: " << camera_info;
   m_camera = new QCamera(camera_info);
   m_camera->setViewfinder(m_ui.viewFinder);
   m_imageCapture = new QCameraImageCapture(m_camera, m_camera);
   m_camera->setCaptureMode(QCamera::CaptureStillImage);
   m_imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
   QList<QVideoFrame::PixelFormat> pfmts = m_imageCapture->supportedBufferFormats();
   qDebug() << "Supported Formats: " << pfmts;
   m_ui.captureImageBtn->setEnabled(false);
   m_camera->start();
   qDebug() << "Connecting signals";
   m_ui.keepBtn->setEnabled(false);
   connect(this, &QDialog::accepted, this, &CameraCaptureDialog::closeDialog);
   connect(this, &QDialog::rejected, this, &CameraCaptureDialog::closeDialog);
   connect(m_ui.keepBtn, &QPushButton::clicked, this, &QDialog::accept);
   connect(m_ui.cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
   connect(m_ui.cameraSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionChanged()));
   connect(m_ui.captureImageBtn, SIGNAL(clicked()), this, SLOT(captureImage()));
   connect(m_ui.captureImageBtn, SIGNAL(pressed()), this, SLOT(focusImage()));
   connect(m_imageCapture, &QCameraImageCapture::imageCaptured, this, &CameraCaptureDialog::processCapturedImage);
   connect(m_imageCapture, &QCameraImageCapture::readyForCaptureChanged, this, &CameraCaptureDialog::readyForCapture);
}

CameraCaptureDialog::~CameraCaptureDialog()
{
   delete m_camera;
}

void CameraCaptureDialog::closeDialog()
{
   qDebug() << "Stopping camera";
   m_camera->stop();
}

void CameraCaptureDialog::keepImage()
{
   qDebug() << "Stopping camera";
   m_camera->stop();
}

void CameraCaptureDialog::captureImage()
{
   qDebug() << "Stopping camera";

   m_imageCapture->capture();
}

void CameraCaptureDialog::discardImage()
{
   qDebug() << "Stopping camera";
   m_camera->stop();
   delete m_camera;
}

void CameraCaptureDialog::focusImage()
{
   qDebug() << "focus mode" << m_camera->focus()->focusMode();
}

QByteArray CameraCaptureDialog::getImage(const QByteArray &format, int compression)
{
   QBuffer rv;
   QImageWriter img_writer(&rv, format);
   img_writer.setCompression(compression);
   img_writer.write(m_image);
   return(rv.buffer());
}

void CameraCaptureDialog::processCapturedImage(int requestId, const QImage& img)
{
    Q_UNUSED(requestId);
    qDebug() << "imageSize = " << img.size() 
             << "view finder size: " << m_ui.viewFinder->size();
    m_image = img;
    m_hasImage = true;
   m_ui.keepBtn->setEnabled(true);
#if 0
    QImage scaledImage = img.scaled(ui->viewfinder->size(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation);

    ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));

    // Display captured image for 4 seconds.
    displayCapturedImage();
    QTimer::singleShot(4000, this, &Camera::displayViewfinder);
#endif
}

void CameraCaptureDialog::readyForCapture(bool ready)
{
   qDebug() << "ready? " << ready;
   if (ready)
   {
      m_ui.captureImageBtn->setEnabled(ready);
   }
}

void CameraCaptureDialog::selectionChanged()
{
   delete m_camera;
   QByteArray dev_name = m_ui.cameraSelect->currentData().toByteArray();
   qDebug() << "Current device: " << dev_name;
   m_camera = new QCamera(dev_name);
   m_camera->setViewfinder(m_ui.viewFinder);
   m_imageCapture = new QCameraImageCapture(m_camera, m_camera);
   m_camera->setCaptureMode(QCamera::CaptureStillImage);
   m_camera->start();
}
