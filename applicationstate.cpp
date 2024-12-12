#include "applicationstate.h"


void ApplicationState::setGuiState(GuiState guiState) {
    if (this->m_guiState == guiState) {
        return;
    }
    this->m_guiState = guiState;
    emit guiStateChanged();
}


void ApplicationState::setRootDirectory(const QString &rootDirectory) {
    if (this->m_rootDirectory == rootDirectory) {
        return;
    }
    this->m_rootDirectory = rootDirectory;
    emit rootDirectoryChanged();
}


void ApplicationState::setScanResult(const ScanResult &scanResult) {
    this->m_scanResult = scanResult;
    emit scanResultChanged();
}

void ApplicationState::setCancelRequest(bool cancelRequest) {
    if (this->m_cancelRequest == cancelRequest) {
        return;
    }
    this->m_cancelRequest = cancelRequest;
    emit cancelRequestChanged();
}



