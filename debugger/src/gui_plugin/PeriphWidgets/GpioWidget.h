/**
 * @file
 * @copyright  Copyright 2016 GNSS Sensor Ltd. All right reserved.
 * @author     Sergey Khabarov - sergeykhbr@gmail.com
 * @brief      LED's emulator.
 */

#pragma once

#include "api_core.h"   // MUST BE BEFORE QtWidgets.h or any other Qt header.
#include "attribute.h"
#include "igui.h"
#include "coreservices/isocinfo.h"

#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMdiSubWindow>
#include <QtWidgets/QAction>
#include <QtCore/QEvent>

namespace debugger {

class GpioWidget : public QWidget,
                   public IGuiCmdHandler {
    Q_OBJECT
public:
    GpioWidget(IGui *igui, QWidget *parent);
    virtual ~GpioWidget();

    /** IGuiCmdHandler */
    virtual void handleResponse(AttributeType *req, AttributeType *resp);

signals:
    void signalClose(QWidget *, AttributeType &);
    void signalLedValue(uint32_t value);
    void signalDipValue(uint32_t value);

private slots:
    void slotUpdateByTimer();

private:
    IGui *igui_;

    AttributeType cmdRd_;
    GpioType value_;
    GpioType newValue_;
};

class GpioQMdiSubWindow : public QMdiSubWindow {
    Q_OBJECT
public:
    GpioQMdiSubWindow(IGui *igui, QMdiArea *area, QWidget *parent,
                      QAction *act)
        : QMdiSubWindow(parent) {
        setAttribute(Qt::WA_DeleteOnClose);
        action_ = act;
        area_ = area;

        setWindowTitle(tr("gpio0"));
        setWindowIcon(QIcon(tr(":/images/gpio_96x96.png")));
        QWidget *pnew = new GpioWidget(igui, this);
        act->setChecked(true);
        connect(parent, SIGNAL(signalUpdateByTimer()),
                pnew, SLOT(slotUpdateByTimer()));
        setWidget(pnew);
        area_->addSubWindow(this);
        show();
    }
    
protected:
    void closeEvent(QCloseEvent *event_) Q_DECL_OVERRIDE {
        action_->setChecked(false);
        area_->removeSubWindow(this);
        event_->accept();
    }
private:
    QAction *action_;
    QMdiArea *area_;
};

}  // namespace debugger
