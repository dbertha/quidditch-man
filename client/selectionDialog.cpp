#include "selectionDialog.hpp"

SelectionDialog::SelectionDialog(const QStringList &items,QWidget *parent)
    : QDialog(parent), row(-1) {
    model = new QStringListModel(this);
    model->setStringList(items);
    listView = new QListView;
    listView->setModel(model);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    selectButton = new QPushButton(tr("Select Current"));
    selectButton->setDefault(true);
    cancelButton= new QPushButton(tr("Cancel"));
    bottomLayout = new QHBoxLayout;
    if (items.length()>0) {
        bottomLayout->addWidget(selectButton);
        connect(selectButton, SIGNAL(clicked()), this, SLOT(select()));
    }
    bottomLayout->addWidget(cancelButton);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(listView);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

void SelectionDialog::select() {
    row = listView->currentIndex().row();
    accept();
}

int SelectionDialog::getPosition() {return row;}
