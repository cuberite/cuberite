#include "Globals.h"
#include "GeneratorSetupDlg.h"
#include <QLabel>
#include <QLineEdit>
#include "Generating/BioGen.h"
#include "inifile/iniFile.h"





static const QString s_GeneratorNames[] =
{
	QString("Checkerboard"),
	QString("Constant"),
	QString("DistortedVoronoi"),
	QString("MultiStepMap"),
	QString("TwoLevel"),
	QString("Voronoi"),
};





GeneratorSetupDlg::GeneratorSetupDlg(const AString & a_IniFileName, QWidget * a_Parent) :
	super(a_Parent),
	m_IniFile(new cIniFile())
{
	// The generator name is in a separate form layout at the top, always present:
	m_cbGenerator = new QComboBox();
	m_cbGenerator->setMinimumWidth(300);
	for (size_t i = 0; i < ARRAYCOUNT(s_GeneratorNames); i++)
	{
		m_cbGenerator->addItem(s_GeneratorNames[i]);
	}
	QFormLayout * nameLayout = new QFormLayout();
	nameLayout->addRow(new QLabel(tr("Generator")), m_cbGenerator);

	// The rest of the controls are in a dynamically created form layout:
	m_FormLayout = new QFormLayout();

	// The main layout joins these two vertically:
	m_MainLayout = new QVBoxLayout();
	m_MainLayout->addLayout(nameLayout);
	m_MainLayout->addLayout(m_FormLayout);
	setLayout(m_MainLayout);

	// Load the INI file, if specified, otherwise set defaults:
	if (!a_IniFileName.empty() && m_IniFile->ReadFile(a_IniFileName))
	{
		m_cbGenerator->setCurrentText(QString::fromStdString(m_IniFile->GetValue("Generator", "BiomeGen")));
	}
	else
	{
		m_IniFile->SetValue("Generator", "Generator", "Composable");
		m_IniFile->SetValue("Generator", "BiomeGen", m_cbGenerator->currentText().toStdString());
		bool dummy;
		delete cBiomeGen::CreateBiomeGen(*m_IniFile, m_Seed, dummy);
	}
	updateFromIni();

	// Connect the combo change even only after the data has been loaded:
	connect(m_cbGenerator, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(generatorChanged(QString)));
}





void GeneratorSetupDlg::generatorChanged(const QString & a_NewName)
{
	// Clear the current contents of the form layout by assigning it to a stack temporary:
	{
		m_MainLayout->takeAt(1);
		QWidget().setLayout(m_FormLayout);
	}

	// Re-create the layout:
	m_FormLayout = new QFormLayout();
	m_MainLayout->addLayout(m_FormLayout);

	// Recreate the INI file:
	m_IniFile->Clear();
	m_IniFile->SetValue("Generator", "Generator", "Composable");
	m_IniFile->SetValue("Generator", "BiomeGen", a_NewName.toStdString());

	// Create a dummy biome gen from the INI file, this will create the defaults in the INI file:
	bool dummy;
	delete cBiomeGen::CreateBiomeGen(*m_IniFile, m_Seed, dummy);

	// Read all values from the INI file and put them into the form layout:
	updateFromIni();
}





void GeneratorSetupDlg::updateFromIni()
{
	int keyID = m_IniFile->FindKey("Generator");
	if (keyID <= -1)
	{
		return;
	}
	int numItems = m_IniFile->GetNumValues(keyID);
	for (int i = 0; i < numItems; i++)
	{
		AString itemName  = m_IniFile->GetValueName(keyID, i);
		AString itemValue = m_IniFile->GetValue(keyID, i);
		if ((itemName == "Generator") || (itemName == "BiomeGen"))
		{
			// These special cases are not to be added
			continue;
		}
		QLineEdit * edit = new QLineEdit();
		edit->setText(QString::fromStdString(itemValue));
		m_FormLayout->addRow(new QLabel(QString::fromStdString(itemName)), edit);
	}  // for i - INI values[]
}




