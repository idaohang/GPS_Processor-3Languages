/* 
 * File:   XMLCreator.cpp
 * Author: jee22
 * 
 * Created on 21 March 2014, 02:30
 */

#include "XMLCreator.h"
#include <sstream>
#include <iostream>
#include <string>
#include "Stream.h"

using namespace std;

XMLCreator::XMLCreator() {
    file_handler = new FileHandler("cplusplus_output.gpx", 'w');
}

XMLCreator::XMLCreator(const XMLCreator& orig) {
}

XMLCreator::~XMLCreator() {
    delete(file_handler);
}

void XMLCreator::startXML(){
    file_handler->openInitialWrite();
    string xml_tag;
    xml_tag = xml_tag = "<gpx version=\"1.0\"\n"
            "creator=\"ExpertGPS 1.1 - http://www.topografix.com\"\n"
            "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
            "xmlns=\"http://www.topografix.com/GPX/1/0\"\n"
            "xsi:schemaLocation=\"http://www.topografix.com/GPX/1/0 "
            "http://www.topografix.com/GPX/1/0/gpx.xsd\">\n";
    
    file_handler->writeFile(xml_tag);
    file_handler->closeWriteFile();
}

/**
 * The end of the GPX XML file.
 */
void XMLCreator::endXML() {
    file_handler->openWriteFile();
    string xml_tag;
    xml_tag = "</gpx>\n";
    file_handler->writeFile(xml_tag);
    file_handler->closeWriteFile();
}

/**
 * Take a time and put it into a format for the GPX XML
 * file.
 * @param date_and_time The time to be formatted.
 */
string XMLCreator::tagDateAndTime(time_t date_and_time) {
    time_t to_format = date_and_time;
    char buffer[40];
    /* The time is formated using strftime, based on how it should be
     for the GPX XML file.*/
    strftime(buffer, 40, "<time>%Y-%m-%dT%H:%M:%SZ</time>\n", localtime(&to_format));
    string xml_tag;
    xml_tag = buffer;

    return xml_tag;
}

string XMLCreator::tagWaypoint(double latitude, double longitude){
    string start = "<wpt lat=\"";
    string mid = "\" lon=\"";
    string end = "\">\n";
    
    ostringstream convert_lat;
    convert_lat.precision(7);
    convert_lat << latitude;
    string lat_string = convert_lat.str();
    ostringstream convert_lng;
    convert_lng.precision(7);
    convert_lng << longitude;
    string lng_string = convert_lng.str();
    
    return start + lat_string + mid + lng_string + end;
}

string XMLCreator::tagElevation(double elevation){
    string xml_tag_start = "<ele>";
    string xml_tag_end = "</ele>\n";
    
    ostringstream convert;
    convert << elevation;
    string ele_string = convert.str();
    
    return xml_tag_start + ele_string + xml_tag_end;
}

string XMLCreator::tagEndWaypoint(){
    return "</wpt>\n";
}

/**
 * A call to output the different sections of a Way point for
 * the particular stream and it's current point.
 * @param streamer The stream to be output.
 */
void XMLCreator::outputStream(Stream * streamer) {
    file_handler->openWriteFile();
    file_handler->writeFile(tagWaypoint(streamer->getLatitude(), streamer->getLongitude()));
    file_handler->writeFile(tagElevation(streamer->getElevation()));
    file_handler->writeFile(tagDateAndTime(streamer->getDateAndTime()));
    file_handler->writeFile(tagEndWaypoint());
    file_handler->closeWriteFile();
}