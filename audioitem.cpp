#include "audioitem.h"

const QStringList AudioItem::extensions =
        QStringList() << ".mka" << ".ogg" << ".wav" << ".mp2" << ".mp3" << ".wma" << ".ac3" <<
                         ".dts" << ".aac" << ".ape" << ".mac" << ".flac"<< ".dat" << ".aiff" <<
                         ".aifc" << ".au" << ".iff" << ".paf" << ".sd2" << ".irca" << ".w64" <<
                         ".mat" << ".pvf" << ".xi" << ".sds" << ".avr";

bool AudioItem::extensionExists(QString &extension){
    foreach(QString ext, extensions){
        if(QString::compare(ext,extension,Qt::CaseInsensitive) == 0){
            return true;
        }
    }

    return false;
}

AudioItem::AudioItem(QString &filePath, QObject *parent):
    MediaItem(filePath, parent)
{
}

QString AudioItem::getNonFormattedResult(){
    Data d;

    getData(d);

    QString retval = "";

    retval += getFileName() + "\r\n";
    retval += " " + tr("Codec:") + " " + d.audioCodec + "\r\n";
    retval += " " + tr("Duration:") + " " + MediaItem::formatSeconds(d.duration/1000) + "\r\n";
    retval += " " + tr("File size:") + " " + MediaItem::formatBytes(d.fileSize) + "\r\n";
    retval += " " + tr("Bitrate:") + " " + MediaItem::formatBits(d.bitRate) + "/s " + d.bitRateMode + "\r\n";
    retval += " " + tr("Sampling rate:") + " " + MediaItem::formatRate(d.samplingRate) + "\r\n";
    retval += " " + tr("Channels:") + " " + QString("%1\r\n\r\n").arg(d.channels);

    return retval;
}

QString AudioItem::getBBFormattedResult(){
    Data d;

    getData(d);

    QString retval = "";

    retval += "[b]" + getFileName() + "[/b]\r\n";
    retval += " " + tr("Codec:") + " " + d.audioCodec + "\r\n";
    retval += " " + tr("Duration:") + " " + MediaItem::formatSeconds(d.duration/1000) + "\r\n";
    retval += " " + tr("File size:") + " " + MediaItem::formatBytes(d.fileSize) + "\r\n";
    retval += " " + tr("Bitrate:") + " " + MediaItem::formatBits(d.bitRate) + "/s " + d.bitRateMode + "\r\n";
    retval += " " + tr("Sampling rate:") + " " + MediaItem::formatRate(d.samplingRate) + "\r\n";
    retval += " " + tr("Channels:") + " " + QString("%1\r\n\r\n").arg(d.channels);

    return retval;
}

QString AudioItem::getHtmlFormattedResult(){
    Data d;

    getData(d);

    QString retval = "";

    retval += "<b>" + getFileName() + "</b><br />\r\n";
    retval += " " + tr("Codec:") + " " + d.audioCodec + "<br />\r\n";
    retval += " " + tr("Duration:") + " " + MediaItem::formatSeconds(d.duration/1000) + "<br />\r\n";
    retval += " " + tr("File size:") + " " + MediaItem::formatBytes(d.fileSize) + "<br />\r\n";
    retval += " " + tr("Bitrate:") + " " + MediaItem::formatBits(d.bitRate) + "/s " + d.bitRateMode + "<br />\r\n";
    retval += " " + tr("Sampling rate:") + " " + MediaItem::formatRate(d.samplingRate) + "<br />\r\n";
    retval += " " + tr("Channels:") + " " + QString("%1<br /><br />\r\n\r\n").arg(d.channels);

    return retval;
}

void AudioItem::getData(Data &d){
    if(data->hasChildNodes() && data->firstChild().hasChildNodes()){
        QDomNodeList list = data->firstChild().childNodes().at(0).childNodes();
        for(int i = 0; i < list.size(); i++){
            QDomElement element = list.at(i).toElement();

            if(element.nodeName() == "File_size"){
                if (d.fileSize == 0)
                {
                    d.fileSize = element.text().toLong();
                }
            }
        }

        list = data->firstChild().childNodes().at(1).childNodes();
        for(int i = 0; i < list.size(); i++){
            QDomElement element = list.at(i).toElement();

            if(element.nodeName() == "Codec")
            {
                d.audioCodec = element.text();
            }
            else if(element.nodeName() == "Duration")
            {
                if(d.duration == 0)
                {
                    d.duration = element.text().toInt();
                }
            }
            else if(element.nodeName() == "Bit_rate"){
                if(d.bitRate == 0)
                {
                    d.bitRate = element.text().toInt();
                }
            }
            else if(element.nodeName() == "Bit_rate_mode"){
                if (d.bitRateMode == "")
                {
                    d.bitRateMode = element.text();
                }
            }
            else if(element.nodeName() == "Channel_s_"){
                if (d.channels == 0)
                {
                    d.channels = element.text().toInt();
                }
            }
            else if(element.nodeName() == "Channel_count"){
                if (d.channels == 0)
                {
                    d.channels = element.text().toInt();
                }
            }
            else if(element.nodeName() == "Sampling_rate"){
                if (d.samplingRate == 0)
                {
                    d.samplingRate = element.text().toInt();
                }
            }
        }
    }
}

AudioItem::Data::Data(){
    duration = bitRate = samplingRate = channels = fileSize = 0;
}
