#include "AddActorDialog.hpp"
#include "ui_add_actor_dialog.h"
#include <boost/foreach.hpp>

namespace omviz
{

AddActorDialog::AddActorDialog(QWidget* parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
    , mUi(new Ui::AddActorDialog)
{
    mUi->setupUi(this);

    mUi->lineEdit_ModelLabel->setEnabled(false);
    connect(mUi->buttonBox_AddActor, SIGNAL(accepted()), this, SLOT(checkAndStoreValues()));
}

AddActorDialog::~AddActorDialog()
{
    delete mUi;
}

void AddActorDialog::setModelList(const owlapi::model::IRIList& iriList)
{
    BOOST_FOREACH(const owlapi::model::IRI& iri, iriList)
    {
        mUi->comboBox_Models->addItem( QString( iri.getFragment().c_str()) );
    }
}

void AddActorDialog::checkAndStoreValues()
{
    //QString modelLabel = mUi->lineEdit_ModelLabel->text();
    QString modelType = mUi->comboBox_Models->currentText();
    if( /*modelLabel != QString("") && */ modelType != QString(""))
    {
        //mInstance = owlapi::vocabulary::OM::resolve(modelLabel.toStdString());
        mModel = owlapi::vocabulary::OM::resolve(modelType.toStdString());
    }
}

} // end namespace omviz
