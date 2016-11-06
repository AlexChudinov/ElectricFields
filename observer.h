#ifndef OBSERVER_H
#define OBSERVER_H

#include <QWidget>

/**
 * Observation of application processes
 */
class QProgressBar;

class Observer : public QWidget
{
    Q_OBJECT
public:
    Observer(QWidget* parent = 0);
    ~Observer();

    QProgressBar* progressBar() const;
    QString taskName() const;

public slots:
    void initProgress(QString taskName = QString());
    void setProgressVal(int val);
    void hideProgressBar();

signals:
    void hiden(bool);

private:
    QProgressBar* m_progressBar;
    QString       m_strTaskName;
};

#endif // OBSERVER_H
