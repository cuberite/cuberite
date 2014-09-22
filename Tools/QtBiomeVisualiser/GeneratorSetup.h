#pragma once

#include <memory>
#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QFormLayout>





class cIniFile;
typedef std::shared_ptr<cIniFile> cIniFilePtr;





class GeneratorSetup :
	public QWidget
{
	typedef QWidget super;

	Q_OBJECT

public:
	/** Creates the widget and loads the contents of the INI file, if not empty. */
	explicit GeneratorSetup(const std::string & a_IniFileName, QWidget * parent = nullptr);

	/** Returns the cIniFile instance that is being edited by this widget. */
	cIniFilePtr getIniFile() { return m_IniFile; }

signals:
	/** Emitted when the generator parameters have changed. */
	void generatorUpdated();

public slots:
	/** Called when the user selects a different generator from the top combobox.
	Re-creates m_IniFile and updates the form layout. */
	void generatorChanged(const QString & a_NewName);

protected slots:
	/** Called when any of the edit widgets are changed. */
	void editChanged(const QString & a_NewValue);

protected:
	QComboBox * m_cbGenerator;
	QLineEdit * m_eSeed;
	QVBoxLayout * m_MainLayout;
	QFormLayout * m_FormLayout;

	cIniFilePtr m_IniFile;

	int m_Seed;


	/** Updates the form layout with the values from m_IniFile. */
	void updateFromIni();
};




