#include "crack.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileInfo>
#include <QUrl>
#include <QWidget>
#include <QPushButton>
#include <QInputDialog>
#include <QLineEdit>
#include <QLabel>
#include <iostream>
#include <string>

const char* MUSIC_FILE_NAME = "mus2.wav";
bool was_file_patched(String* buffer);

//void init_crack(String* buffer, FILE* p_input_file);
//void show_gui(EXEC_MODE mode);

void initString(String* buffer){

	buffer->data = (char*)calloc(max_buf_len, sizeof(char));
	buffer->len  = 0;

	return;
}

void destrString(String* buffer){

	free(buffer->data);
	buffer->len = 0;

	return;
}

void read_buffer(FILE* p_input_file, String* buffer){

	size_t n_read = fread(buffer->data, sizeof(char), max_buf_len, p_input_file);

	buffer->len = n_read;
	return;
}

void Crack::CrackLaunch(){

	std::string input_file_name = 	qPrintable(Crack::text_field->text());

	FILE* p_input_file = fopen(input_file_name.c_str(), "rb");				// обработать

	slowDebugPrint("Trying read file");

	if(p_input_file == NULL){
		Crack::text_field->setText("No such file");
		return;
	}

	slowDebugPrint("success");

	String buffer = {};

	initString(&buffer);
	read_buffer(p_input_file, &buffer);
	fclose(p_input_file);

	 if(was_file_patched(&buffer)){
	 		slowDebugPrint("File was already patched");
	 		return;
	 }

	slowDebugPrint("Converting to buffer completed");

	buffer.data[offset_first_check]      = 0x90;
	buffer.data[offset_first_check + 1]  = 0x90;
	
	buffer.data[offset_second_check]     = 0x90;
	buffer.data[offset_second_check + 1] = 0x90;
	
	slowDebugPrint("Data was patched");

	slowDebugPrint("Trying to write data");
	
	FILE* p_output_file = fopen(input_file_name.c_str(), "wb");

	fwrite(buffer.data, sizeof(char), buffer.len, p_output_file);

	fclose(p_output_file);

	slowDebugPrint("success");
	
	destrString(&buffer);

	slowDebugPrint("File was patched");
	
	return;
}

int WindowInit(int argc, char **argv){

	QApplication app(argc, argv);
	QWidget window;

	window.setFixedSize(WINDOW_WIDTH, WINDOW_HEIGTH);

	QPushButton *sound_button = new QPushButton("sound", &window);
 	sound_button->setGeometry(SOUND_BUT_X, BUT_Y, BUT_WIDTH, BUT_HEIGHT);

	QPushButton *crack_button = new QPushButton("crack", &window);
 	crack_button->setGeometry(CRACK_BUT_X, BUT_Y, BUT_WIDTH, BUT_HEIGHT);
	
	QMediaPlaylist *list = new QMediaPlaylist;
	list->addMedia(QUrl::fromLocalFile(QFileInfo(MUSIC_FILE_NAME).absoluteFilePath()));
	
	QMediaPlayer *music;
	music == new QMediaPlayer();
	music->setPlaylist(list);
	music->play();

	QLabel *label = new QLabel(&window);
 	label->setGeometry(50, 20, IMAGE_WIDTH, IMAGE_HEITH);
	QPixmap pic("bg_img.jpg");
	label->setPixmap(pic);
	
	QLabel *label2 = new QLabel(&window);
	label2->setText("insert the file name: ");
	label2->setGeometry(INSERT_TEXT_X, BUT_Y, 140, BUT_HEIGHT);

	Crack* crack = new Crack(&window);

	QObject::connect(sound_button, SIGNAL (clicked()), music, SLOT(stop()));

	QObject::connect(crack_button, SIGNAL (clicked()), crack, SLOT(CrackLaunch())); 	

 	window.show();
	return app.exec();
}

bool was_file_patched(String* buffer){

	assert(buffer != NULL);

	int counter = 0;

	for(size_t i = 0; i < buffer->len; i++){
		counter += buffer->data[i] % 255;
	}

	if(counter == ETALON_COUNTER) return 1;

	return 0;
}
