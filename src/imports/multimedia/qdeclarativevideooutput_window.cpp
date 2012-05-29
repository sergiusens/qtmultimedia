/****************************************************************************
**
** Copyright (C) 2012 Research In Motion
** Contact: http://www.qt-project.org/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeclarativevideooutput_window_p.h"
#include "qdeclarativevideooutput_p.h"
#include <QtQuick/qquickcanvas.h>
#include <QtMultimedia/qmediaservice.h>
#include <QtMultimedia/qvideowindowcontrol.h>

QT_BEGIN_NAMESPACE

QDeclarativeVideoWindowBackend::QDeclarativeVideoWindowBackend(QDeclarativeVideoOutput *parent)
    : QDeclarativeVideoBackend(parent)
{
}

QDeclarativeVideoWindowBackend::~QDeclarativeVideoWindowBackend()
{
    releaseSource();
    releaseControl();
}

bool QDeclarativeVideoWindowBackend::init(QMediaService *service)
{
    if (QMediaControl *control = service->requestControl(QVideoWindowControl_iid)) {
        if ((m_videoWindowControl = qobject_cast<QVideoWindowControl *>(control))) {
            if (q->canvas())
                m_videoWindowControl->setWinId(q->canvas()->winId());
            m_service = service;
            QObject::connect(m_videoWindowControl.data(), SIGNAL(nativeSizeChanged()),
                             q, SLOT(_q_updateNativeSize()));
            return true;
        }
    }
    return false;
}

void QDeclarativeVideoWindowBackend::itemChange(QQuickItem::ItemChange change,
                                                const QQuickItem::ItemChangeData &changeData)
{
    if (change == QQuickItem::ItemSceneChange && m_videoWindowControl) {
        if (changeData.canvas)
            m_videoWindowControl->setWinId(changeData.canvas->winId());
        else
            m_videoWindowControl->setWinId(0);
    }
}

void QDeclarativeVideoWindowBackend::releaseSource()
{
}

void QDeclarativeVideoWindowBackend::releaseControl()
{
    if (m_videoWindowControl) {
        m_videoWindowControl->setWinId(0);
        if (m_service)
            m_service->releaseControl(m_videoWindowControl);
        m_videoWindowControl = 0;
    }
}

QSize QDeclarativeVideoWindowBackend::nativeSize() const
{
    return m_videoWindowControl->nativeSize();
}

void QDeclarativeVideoWindowBackend::updateGeometry()
{
    switch (q->fillMode()) {
    case QDeclarativeVideoOutput::PreserveAspectFit:
        m_videoWindowControl->setAspectRatioMode(Qt::KeepAspectRatio); break;
    case QDeclarativeVideoOutput::PreserveAspectCrop:
        m_videoWindowControl->setAspectRatioMode(Qt::KeepAspectRatioByExpanding); break;
    case QDeclarativeVideoOutput::Stretch:
        m_videoWindowControl->setAspectRatioMode(Qt::IgnoreAspectRatio); break;
    };

    const QRectF canvasRect = q->mapRectToScene(QRectF(0, 0, q->width(), q->height()));
    m_videoWindowControl->setDisplayRect(canvasRect.toAlignedRect());
}

QSGNode *QDeclarativeVideoWindowBackend::updatePaintNode(QSGNode *oldNode,
                                                         QQuickItem::UpdatePaintNodeData *data)
{
    Q_UNUSED(oldNode);
    Q_UNUSED(data);
    m_videoWindowControl->repaint();
    return 0;
}

QAbstractVideoSurface *QDeclarativeVideoWindowBackend::videoSurface() const
{
    return 0;
}

QT_END_NAMESPACE