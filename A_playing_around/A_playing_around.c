#pragma warning(disable:4996) //disabling warning

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>


typedef enum { English, Italian, Spanish } language;
const char *month(int number, language lang);
const char *lang2str(language lang);
int main()
{
	char *wha = NULL;
	wha = lang2str(English);
	printf("%s\n", wha);
	wha = month(2, English);
	printf("%s\n", wha);
	return 0;
}

const char *month(int number, language lang) {
	switch (number)
	{
	case 1:
		switch (lang) {
		case English:
			return "January";
		case Italian:
			return "Gennaio";
		case Spanish:
			return "enero";
		default:
			return "Error";
		}
	case 2:
		switch (lang) {
		case English:
			return "February";
		case Italian:
			return "Febbraio";
		case Spanish:
			return "febrero";
		default:
			return "Error";
		}
	case 3:
		switch (lang) {
		case English:
			return "March";
		case Italian:
			return "Marzo";
		case Spanish:
			return "marzo";
		default:
			return "Error";
		}
	case 4:
		switch (lang) {
		case English:
			return "April";
		case Italian:
			return "Aprile";
		case Spanish:
			return "abril";
		default:
			return "Error";
		}
	case 5:
		switch (lang) {
		case English:
			return "May";
		case Italian:
			return "Maggio";
		case Spanish:
			return "mayo";
		default:
			return "Error";
		}
	case 6:
		switch (lang) {
		case English:
			return "June";
		case Italian:
			return "Giugno";
		case Spanish:
			return "junio";
		default:
			return "Error";
		}
	case 7:
		switch (lang) {
		case English:
			return "July";
		case Italian:
			return "Luglio";
		case Spanish:
			return "julio";
		default:
			return "Error";
		}
	case 8:
		switch (lang) {
		case English:
			return "August";
		case Italian:
			return "Agosto";
		case Spanish:
			return "agosto";
		default:
			return "Error";
		}
	case 9:
		switch (lang) {
		case English:
			return "September";
		case Italian:
			return "Settembre";
		case Spanish:
			return "septiembre";
		default:
			return "Error";
		}
	case 10:
		switch (lang) {
		case English:
			return "October";
		case Italian:
			return "Ottobre";
		case Spanish:
			return "octubre";
		default:
			return "Error";
		}
	case 11:
		switch (lang) {
		case English:
			return "November";
		case Italian:
			return "Novembre";
		case Spanish:
			return "noviembre";
		default:
			return "Error";
		}
	case 12:
		switch (lang) {
		case English:
			return "December";
		case Italian:
			return "Dicembre";
		case Spanish:
			return "diciembre";
		default:
			return "Error";
		}
	default:
		return "Error";
	}
}

const char *lang2str(language lang) {
	switch (lang)
	{
	case English:
		return "English";
	case Italian:
		return "Italian";
	case Spanish:
		return "Spanish";
	default:
		return "Error";
	}
}