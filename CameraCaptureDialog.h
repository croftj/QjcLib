#ifndef QCJLIB_CAMERA_CAPTURE_DIALOG_H
#define QCJLIB_CAMERA_CAPTURE_DIALOG_H

#include <QBuffer>
#include <QByteArray>
#include <QCamera>
#include <QCameraImageCapture>
#include <QDialog>

#include "ui_cameracapturedialog.h"

namespace QcjLib
{
   class CameraCaptureDialog : public QDialog
   {
      Q_OBJECT

   public:
      CameraCaptureDialog(QWidget *parent = nullptr);
      ~CameraCaptureDialog();

      QByteArray getImage(const QByteArray &format, int compression = 1);
      bool hasImage() { return(m_hasImage); };
//      void capture() {};

//      QByteArray imageData() {};

   protected slots:
      void closeDialog();
      void captureImage();
      void discardImage();
      void focusImage();
      void keepImage();
      void processCapturedImage(int requestId, const QImage& img);
      void readyForCapture(bool ready);
      void selectionChanged();
      
   private:
      bool                 m_hasImage;
      QCamera*             m_camera;
      QImage               m_image;
      QCameraImageCapture* m_imageCapture;
      Ui::CameraCaptureDialog m_ui;
   };
}

#endif
