#include "observer.h"
#include <QProgressBar>

Observer::Observer(QWidget *parent)
    :
      QWidget(parent),
      m_progressBar(new QProgressBar(this))
{
    m_progressBar->hide();
}

Observer::~Observer()
{}

QProgressBar* Observer::progressBar() const
{
    return m_progressBar;
}

QString Observer::taskName() const
{
    return m_strTaskName;
}

void Observer::initProgress(QString taskName)
{
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_strTaskName = taskName;
}

void Observer::setProgressVal(int val)
{
    m_progressBar->setValue(val);
}

void Observer::hideProgressBar()
{
    m_progressBar->hide();
    emit this->hiden(true);
}
