/******************************************************************************/
/* $Id$                              */
/*                                                                            */
/* Author: Joe Croft, joe@croftj.net 2023                                     */
/*                                                                            */
/* This is free and unencumbered software released into the public domain.    */
/*                                                                            */
/* Anyone is free to copy, modify, publish, use, compile, sell, or distribute */
/* this software, either in source code form or as a compiled binary,         */
/* for any purpose, commercial or non-commercial, and by any means.           */
/*                                                                            */
/* In jurisdictions that recognize copyright laws, the author or authors of   */
/* this software dedicate any and all copyright interest in the software to   */
/* the public domain. We make this dedication for the benefit of the public   */
/* at large and to the detriment of our heirs and successors. We intend       */
/* this dedication to be an overt act of relinquishment in perpetuity of      */
/* all present and future rights to this software under copyright law.        */
/*                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL   */
/* THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER   */
/* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    */
/* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
/*                                                                            */
/* For more information, please refer to <http://unlicense.org/>              */
/******************************************************************************/
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
   connect(this, &QDialog::accepted, this, &CameraCaptureDialog::closeDialog, Qt::UniqueConnection);
   connect(this, &QDialog::rejected, this, &CameraCaptureDialog::closeDialog, Qt::UniqueConnection);
   connect(m_ui.keepBtn, &QPushButton::clicked, this, &QDialog::accept, Qt::UniqueConnection);
   connect(m_ui.cancelBtn, &QPushButton::clicked, this, &QDialog::reject, Qt::UniqueConnection);
   connect(m_ui.cameraSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionChanged()), Qt::UniqueConnection);
   connect(m_ui.captureImageBtn, SIGNAL(clicked()), this, SLOT(captureImage()), Qt::UniqueConnection);
   connect(m_ui.captureImageBtn, SIGNAL(pressed()), this, SLOT(focusImage()), Qt::UniqueConnection);
   connect(m_imageCapture, &QCameraImageCapture::imageCaptured, this, &CameraCaptureDialog::processCapturedImage, Qt::UniqueConnection);
   connect(m_imageCapture, &QCameraImageCapture::readyForCaptureChanged, this, &CameraCaptureDialog::readyForCapture, Qt::UniqueConnection);
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

QByteArray CameraCaptureDialog::getThumbnail(const QByteArray &format, int compression, int width)
{
   QBuffer rv;
   QImageWriter img_writer(&rv, format);
   img_writer.setCompression(compression);
   img_writer.write(m_image.scaledToWidth(width));
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
