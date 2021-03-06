#pragma once

#include "serialterminal_global.h"

#include <coreplugin/ioutputpane.h>
#include <utils/outputformat.h>

#include "serialdevicemodel.h"
#include "combobox.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
class QToolButton;
class QButtonGroup;
class QAbstractButton;
class QComboBox;
QT_END_NAMESPACE

namespace Core { class OutputWindow; }

class ConsoleLineEdit;

namespace SerialTerminal {
namespace Internal {

class SerialControl;
class TabWidget;

class SerialOutputPane : public Core::IOutputPane
{
    Q_OBJECT

public:
    enum CloseTabMode {
        CloseTabNoPrompt,
        CloseTabWithPrompt
    };

    enum BehaviorOnOutput {
        Flash,
        Popup
    };


    SerialOutputPane(Settings& settings);
    ~SerialOutputPane();


    // IOutputPane
    QWidget *outputWidget(QWidget* parent) override;
    QList<QWidget *> toolBarWidgets() const override;
    QString displayName() const override;

    int priorityInStatusBar() const override;
    void clearContents() override;
    void visibilityChanged(bool) override;
    bool canFocus() const override;
    bool hasFocus() const override;
    void setFocus() override;

    bool canNext() const override;
    bool canPrevious() const override;
    void goToNext() override;
    void goToPrev() override;
    bool canNavigate() const override;

    void createNewOutputWindow(SerialControl *rc);

    bool closeTabs(CloseTabMode mode);

    void appendMessage(SerialControl *rc, const QString &out, Utils::OutputFormat format);

private:
    class SerialControlTab {
    public:
        explicit SerialControlTab(SerialControl *serialControl = nullptr,
                                  Core::OutputWindow *window = nullptr);
        SerialControl* serialControl;
        Core::OutputWindow* window;
        // Is the run control stopping asynchronously, close the tab once it finishes
        bool asyncClosing = false;
        BehaviorOnOutput behaviorOnOutput = Flash;
        int inputCursorPosition {0};
        QString inputText;
        QByteArray lineEnd;
    };


    void createToolButtons();

    void contextMenuRequested(const QPoint &pos, int index);

    void enableDefaultButtons();
    void enableButtons(const SerialControl* rc, bool isRunning);
    void tabChanged(int i);

    void slotSerialControlStarted();
    void slotSerialControlFinished();
    void slotSerialControlFinished2(SerialControl* sender);

    bool isRunning() const;

    void activePortNameChanged(int index);
    void activeBaudRateChanged(int index);

    void connectControl();
    void disconnectControl();
    void resetControl();
    void openNewTerminalControl();
    void sendInput();

    bool closeTab(int index, CloseTabMode cm = CloseTabWithPrompt);
    int indexOf(const SerialControl *rc) const;
    int indexOf(const QWidget *outputWindow) const;
    int currentIndex() const;
    SerialControl* currentSerialControl() const;
    int findTabWithPort(QString const& portName) const;
    int findRunningTabWithPort(const QString& portName) const;
    void handleOldOutput(Core::OutputWindow *window) const;

    void updateCloseActions();


    QWidget* m_mainWidget {nullptr};
    ConsoleLineEdit* m_inputLine {nullptr};
    TabWidget* m_tabWidget {nullptr};
    Settings m_settings;
    QVector<SerialControlTab> m_serialControlTabs;
    int m_prevTabIndex {-1};

    SerialDeviceModel* m_devicesModel {nullptr};

    QAction* m_closeCurrentTabAction {nullptr};
    QAction* m_closeAllTabsAction {nullptr};
    QAction* m_closeOtherTabsAction {nullptr};

    QAction* m_disconnectAction {nullptr};
    QToolButton* m_connectButton {nullptr};
    QToolButton* m_disconnectButton {nullptr};
    QToolButton* m_resetButton {nullptr};
    QToolButton* m_newButton {nullptr};
    ComboBox* m_portsSelection {nullptr};
    ComboBox* m_baudRateSelection {nullptr};
};

} // namespace Internal
} // namespace SerialTerminal
