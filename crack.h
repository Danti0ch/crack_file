#pragma once

#ifndef CRACK_H
#define CRACK_H

#include <QObject>
#include <unistd.h>
#include <QLineEdit>
const int max_buf_len         = 1024 * 1024;
const int MAX_FILE_NAME_LEN   = 256;

const int offset_first_check  = 0x4011e8 - 0x400000;
const int offset_second_check = 0x401209 - 0x400000;

const int SOUND_BUT_X 	= 20;
const int CRACK_BUT_X 	= 90;
const int EXIT_BUT_X 	= 180;

const int BUT_Y 		= 350;
const int BUT_WIDTH  	= 60;
const int BUT_HEIGHT 	= 40;

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGTH = 400;

const int IMAGE_WIDTH   = 600;
const int IMAGE_HEITH   = 320;

const int DEBUG_FIELD_X    = 160;
const int DEBUG_FIELD_WIDTH = 200;

const int INSERT_FIELD_X = 510;

const int INSERT_TEXT_X  = 370;

const int ETALON_COUNTER = 125049;

//const char* input_file_name = NULL;
//const crack_status = 0;

#define slowDebugPrint(s)			\
{									\
	Crack::debug_field->setText(s);	\
	sleep(0.5);						\
}

enum EXEC_MODE{
	OK,
	FILE_READING_FAILED,
	NO_FILE_ENTRY
};

struct String{
	char* data;
	size_t len;
};

class Crack : public QObject
{
    Q_OBJECT

public:
    Crack(QWidget* parent){
    	text_field = new QLineEdit("file", parent);
		text_field->setGeometry(INSERT_FIELD_X, BUT_Y, BUT_WIDTH*2, BUT_HEIGHT);

		debug_field = new QLineEdit("hey :)", parent);
		debug_field->setGeometry(DEBUG_FIELD_X, BUT_Y, DEBUG_FIELD_WIDTH, BUT_HEIGHT);
		debug_field->setReadOnly(1);

		is_initiated = 0;
    }

//    int value() const { return m_value; }

public slots:
    void CrackLaunch();

private:
    QLineEdit *text_field;
    QLineEdit *debug_field;
    bool is_initiated;
};

int WindowInit(int argc, char **argv);

#endif //CRACK_H