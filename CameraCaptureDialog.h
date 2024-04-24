/******************************************************************************/
/* $Id$                              */
/*                                                                            */
/* Author: Joe Croft, joe@croftj.net 2023-2024                                     */
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
      QByteArray getThumbnail(const QByteArray &format, int compression = 1, int width = 200);
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
      QImage               m_thumbnail;
      QCameraImageCapture* m_imageCapture;
      Ui::CameraCaptureDialog m_ui;
   };
}

#endif
