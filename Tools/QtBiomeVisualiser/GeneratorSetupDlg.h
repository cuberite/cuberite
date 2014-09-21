#pragma once

#include <memory>
#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QFormLayout>





class cIniFile;





class GeneratorSetupDlg :
	public QDialog
{
	typedef QDialog super;

	Q_OBJECT

public:
	/** Creates the dialog and loads the contents of the INI file, if not empty. */
	explicit GeneratorSetupDlg(const std::string & a_IniFileName, QWidget * parent = nullptr);

signals:

public slots:
	/** Called when the user selects a different generator from the top combobox.
	Re-creates m_IniFile and updates the form layout. */
	void generatorChanged(const QString & a_NewName);

protected:
	QComboBox * m_cbGenerator;
	QVBoxLayout * m_MainLayout;
	QFormLayout * m_FormLayout;

	std::unique_ptr<cIniFile> m_IniFile;

	int m_Seed;


	/** Updates the form layout with the values from m_IniFile. */
	void updateFromIni();
};




