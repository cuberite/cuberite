#include "Globals.h"
#include "GeneratorSetup.h"
#include <QLabel>
#include <QLineEdit>
#include "src/Generating/BioGen.h"
#include "src/IniFile.h"





static const QString s_GeneratorNames[] =
{
	QString("Checkerboard"),
	QString("Constant"),
	QString("DistortedVoronoi"),
	QString("Grown"),
	QString("GrownProt"),
	QString("MultiStepMap"),
	QString("TwoLevel"),
	QString("Voronoi"),
};





GeneratorSetup::GeneratorSetup(const AString & a_IniFileName, QWidget * a_Parent) :
	super(a_Parent),
	m_IniFile(new cIniFile())
{
	// The seed and generator name is in a separate form layout at the top, always present:
	m_eSeed = new QLineEdit();
	m_eSeed->setValidator(new QIntValidator());
	m_eSeed->setText("0");
	m_eSeed->setProperty("INI.SectionName", QVariant("Seed"));
	m_eSeed->setProperty("INI.ItemName", QVariant("Seed"));
	m_cbGenerator = new QComboBox();
	m_cbGenerator->setMinimumWidth(120);
	for (size_t i = 0; i < ARRAYCOUNT(s_GeneratorNames); i++)
	{
		m_cbGenerator->addItem(s_GeneratorNames[i]);
	}
	QFormLayout * baseLayout = new QFormLayout();
	baseLayout->addRow(new QLabel(tr("Seed")), m_eSeed);
	baseLayout->addRow(new QLabel(tr("Generator")), m_cbGenerator);

	// The rest of the controls are in a dynamically created form layout:
	m_FormLayout = new QFormLayout();

	// The main layout joins these two vertically:
	m_MainLayout = new QVBoxLayout();
	m_MainLayout->addLayout(baseLayout);
	m_MainLayout->addLayout(m_FormLayout);
	m_MainLayout->addStretch();
	setLayout(m_MainLayout);

	// Load the INI file, if specified, otherwise set defaults:
	if (a_IniFileName.empty() || !m_IniFile->ReadFile(a_IniFileName))
	{
		m_IniFile->SetValue("Generator", "Generator", "Composable");
		m_IniFile->SetValue("Generator", "BiomeGen", m_cbGenerator->currentText().toStdString());
		bool dummy;
		cBiomeGen::CreateBiomeGen(*m_IniFile, 0, dummy);
	}
	updateFromIni();

	// Connect the change events only after the data has been loaded:
	connect(m_cbGenerator, SIGNAL(currentIndexChanged(QString)), this, SLOT(generatorChanged(QString)));
	connect(m_eSeed,       SIGNAL(textChanged(QString)),         this, SLOT(editChanged(QString)));
}





void GeneratorSetup::generatorChanged(const QString & a_NewName)
{
	// Clear the current contents of the form layout by assigning it to a stack temporary:
	{
		m_MainLayout->takeAt(1);
		QWidget().setLayout(m_FormLayout);
	}

	// Re-create the layout:
	m_FormLayout = new QFormLayout();
	m_MainLayout->insertLayout(1, m_FormLayout);

	// Recreate the INI file:
	m_IniFile->Clear();
	m_IniFile->SetValue("Generator", "Generator", "Composable");
	m_IniFile->SetValue("Generator", "BiomeGen", a_NewName.toStdString());

	// Create a dummy biome gen from the INI file, this will create the defaults in the INI file:
	bool dummy;
	cBiomeGen::CreateBiomeGen(*m_IniFile, m_Seed, dummy);

	// Read all values from the INI file and put them into the form layout:
	updateFromIni();

	// Notify of the changes:
	emit generatorUpdated();
}





void GeneratorSetup::editChanged(const QString & a_NewValue)
{
	QString sectionName = sender()->property("INI.SectionName").toString();
	QString itemName    = sender()->property("INI.ItemName").toString();
	m_IniFile->SetValue(sectionName.toStdString(), itemName.toStdString(), a_NewValue.toStdString());
	emit generatorUpdated();
}





void GeneratorSetup::updateFromIni()
{
	// Set the seed editbox:
	int seed = m_IniFile->GetValueI("Seed", "Seed", 0);
	m_eSeed->setText(QString::number(seed));
	int keyID = m_IniFile->FindKey("Generator");
	if (keyID <= -1)
	{
		return;
	}

	// Set the Generator combobox:
	AString generatorName = m_IniFile->GetValue("Generator", "BiomeGen");
	size_t generatorNameLen = generatorName.length();
	int index = m_cbGenerator->findText(QString::fromStdString(generatorName));
	m_cbGenerator->setCurrentIndex(index);

	// Create the controls for all the generator settings in the INI file:
	int numItems = m_IniFile->GetNumValues(keyID);
	for (int i = 0; i < numItems; i++)
	{
		AString itemName  = m_IniFile->GetValueName(keyID, i);
		if ((itemName == "Generator") || (itemName == "BiomeGen"))
		{
			// These special cases are not to be added
			continue;
		}
		AString itemValue = m_IniFile->GetValue(keyID, i);

		QLineEdit * edit = new QLineEdit();
		edit->setText(QString::fromStdString(itemValue));
		edit->setProperty("INI.SectionName", QVariant("Generator"));
		edit->setProperty("INI.ItemName", QVariant(QString::fromStdString(itemName)));

		// Remove the generator name prefix from the item name, for clarity purposes:
		if (NoCaseCompare(itemName.substr(0, generatorNameLen), generatorName) == 0)
		{
			itemName.erase(0, generatorNameLen);
		}

		connect(edit, SIGNAL(textChanged(QString)), this, SLOT(editChanged(QString)));
		m_FormLayout->addRow(new QLabel(QString::fromStdString(itemName)), edit);
	}  // for i - INI values[]
}




