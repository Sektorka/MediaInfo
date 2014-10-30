#include "videoitem.h"

const QStringList VideoItem::extensions =
        QStringList() << ".avi" << ".mp4" << ".wmv" << ".mkv" << ".ogm" << ".riff"
                      << ".mpeg" << ".mpg" << ".mpgv" << ".mpv" << ".m1v" << ".m2v"
                      << ".vob" << ".m4a" << ".wm" << ".qt" << ".mov"
                      << ".rm" << ".rmvb" << ".ifo" << ".mks" << ".asf";

bool VideoItem::extensionExists(QString &extension){
    foreach(QString ext, extensions){
        if(QString::compare(ext,extension,Qt::CaseInsensitive) == 0){
            return true;
        }
    }

    return false;
}

VideoItem::VideoItem(QString &filePath, QObject *parent) :
    MediaItem(filePath, parent)
{
}

QString VideoItem::getAnalyzeResult() const{
    return "";
}

QString VideoItem::getNonFormattedResult(){
    Data d;

    getData(d);

    QString retval = "";

    retval += "" + getFileName() + "\r\n";
    retval += "   " + tr("File size:") + " " + MediaItem::formatBytes(d.size) + "\r\n";
    retval += "   " + tr("Codec:") + " " + d.codecInfo + " (" + d.codec + ")\r\n\r\n";

    if(d.video.duration != 0 && d.video.size != 0 && d.video.bitRate != 0){
        retval += " " + tr("Video information:") + "\r\n";
        retval += "   " + tr("Codec:") + " " + d.video.videoCodec + "\r\n";
        retval += "   " + tr("Duration:") + " " + MediaItem::formatSeconds(d.video.duration / 1000) + "\r\n";
        retval += "   " + tr("Stream size:") + " " + MediaItem::formatBytes(d.video.size) + "\r\n";
        retval += "   " + tr("Resolution:") + QString(" %1x%2\r\n").arg(d.video.width).arg(d.video.height);
        retval += "   " + tr("Bitrate:") + " " + MediaItem::formatBits(d.video.bitRate) + "/s " + d.video.bitRateMode + "\r\n";
        retval += "   " + tr("FPS:") + " " + QString::number(d.video.framePerSec,'g', -1) + "\r\n";
        retval += "   " + tr("Ratio:") + " " + d.video.ratio + "\r\n\r\n";
    }

    if(d.audios.count() > 0){
        retval += " " + tr("Audio information:") + "\r\n";
        retval += "   " + tr("Number of audio tracks:") + QString(" %1\r\n").arg(d.audios.count());

        int counter = 0;
        foreach (Data::Audio a, d.audios)
        {
            counter++;
            retval += "   " + QString("%1. ").arg(counter) + tr("audio track:") + "\r\n";

            retval += "     " + tr("Codec:") + " " + a.audioCodec + "\r\n";
            retval += "     " + tr("Duration:") + " " + MediaItem::formatSeconds(a.duration / 1000) + "\r\n";
            retval += "     " + tr("Stream size:") + " " + MediaItem::formatBytes(a.size) + "\r\n";
            retval += "     " + tr("Bitrate:") + " " + MediaItem::formatBits(a.bitRate) + "/s " + a.bitRateMode + "\r\n";
            retval += "     " + tr("Sampling rate:") + " " + MediaItem::formatRate(a.samplingRate) + "\r\n";
            retval += "     " + tr("Channels:") + QString(" %1\r\n").arg(a.channels);
        }
    }

    retval += "\r\n\r\n";

    return retval;
}

QString VideoItem::getBBFormattedResult(){
    Data d;

    getData(d);

    QString retval = "";

    retval += "[b]" + getFileName() + "[/b]\r\n";
    retval += "   " + tr("File size:") + " " + MediaItem::formatBytes(d.size) + "\r\n";
    retval += "   " + tr("Codec:") + " " + d.codecInfo + " (" + d.codec + ")\r\n\r\n";

    if(d.video.duration != 0 && d.video.size != 0 && d.video.bitRate != 0){
        retval += " [b][i]" + tr("Video information:") + "[/i][/b]\r\n";
        retval += "   " + tr("Codec:") + " " + d.video.videoCodec + "\r\n";
        retval += "   " + tr("Duration:") + " " + MediaItem::formatSeconds(d.video.duration / 1000) + "\r\n";
        retval += "   " + tr("Stream size:") + " " + MediaItem::formatBytes(d.video.size) + "\r\n";
        retval += "   " + tr("Resolution:") + QString(" %1x%2\r\n").arg(d.video.width).arg(d.video.height);
        retval += "   " + tr("Bitrate:") + " " + MediaItem::formatBits(d.video.bitRate) + "/s " + d.video.bitRateMode + "\r\n";
        retval += "   " + tr("FPS:") + " " + QString::number(d.video.framePerSec,'g', -1) + "\r\n";
        retval += "   " + tr("Ratio:") + " " + d.video.ratio + "\r\n\r\n";
    }

    if(d.audios.count() > 0){
        retval += " [b][i]" + tr("Audio information:") + "[/i][/b]\r\n";
        retval += "   " + tr("Number of audio tracks:") + QString(" %1\r\n").arg(d.audios.count());

        int counter = 0;
        foreach (Data::Audio a, d.audios)
        {
            counter++;
            retval += QString("   [i][u]%1. ").arg(counter) + tr("audio track:") + "[/u][/i]\r\n";

            retval += "     " + tr("Codec:") + " " + a.audioCodec + "\r\n";
            retval += "     " + tr("Duration:") + " " + MediaItem::formatSeconds(a.duration / 1000) + "\r\n";
            retval += "     " + tr("Stream size:") + " " + MediaItem::formatBytes(a.size) + "\r\n";
            retval += "     " + tr("Bitrate:") + " " + MediaItem::formatBits(a.bitRate) + "/s " + a.bitRateMode + "\r\n";
            retval += "     " + tr("Sampling rate:") + " " + MediaItem::formatRate(a.samplingRate) + "\r\n";
            retval += "     " + tr("Channels:") + QString(" %1\r\n").arg(a.channels);
        }
    }

    retval += "\r\n\r\n";

    return retval;
}

QString VideoItem::getHtmlFormattedResult(){
    Data d;

    getData(d);

    QString retval = "";

    retval += "<b>" + getFileName() + "</b><br />\r\n";
    retval += " &nbsp; " + tr("File size:") + " " + MediaItem::formatBytes(d.size) + "<br />\r\n";
    retval += " &nbsp; " + tr("Codec:") + " " + d.codecInfo + " (" + d.codec + ")<br /><br />\r\n\r\n";

    if(d.video.duration != 0 && d.video.size != 0 && d.video.bitRate != 0){
        retval += " <b><i>" + tr("Video information:") + "</i></b><br />\r\n";
        retval += " &nbsp; " + tr("Codec:") + " " + d.video.videoCodec + "<br />\r\n";
        retval += " &nbsp; " + tr("Duration:") + " " + MediaItem::formatSeconds(d.video.duration / 1000) + "<br />\r\n";
        retval += " &nbsp; " + tr("Stream size:") + " " + MediaItem::formatBytes(d.video.size) + "<br />\r\n";
        retval += " &nbsp; " + tr("Resolution:") + QString(" %1x%2\r\n").arg(d.video.width).arg(d.video.height) + "<br />\r\n";
        retval += " &nbsp; " + tr("Bitrate:") + " " + MediaItem::formatBits(d.video.bitRate) + "/s " + d.video.bitRateMode + "<br />\r\n";
        retval += " &nbsp; " + tr("FPS:") + " " + QString::number(d.video.framePerSec,'g', -1) + "<br />\r\n";
        retval += " &nbsp; " + tr("Ratio:") + " " + d.video.ratio + "<br /><br />\r\n\r\n";
    }

    if(d.audios.count() > 0){
        retval += " <b><i>" + tr("Audio information:") + "</i></b><br />\r\n";
        retval += " &nbsp; " + tr("Number of audio tracks:") + QString(" %1<br />\r\n").arg(d.audios.count());

        int counter = 0;
        foreach (Data::Audio a, d.audios)
        {
            counter++;
            retval += QString(" &nbsp; <i><u>%1. ").arg(counter) + tr("audio track:") + "</u></i><br />\r\n";

            retval += " &nbsp; &nbsp; " + tr("Codec:") + " " + a.audioCodec + "<br />\r\n";
            retval += " &nbsp; &nbsp; " + tr("Duration:") + " " + MediaItem::formatSeconds(a.duration / 1000) + "<br />\r\n";
            retval += " &nbsp; &nbsp; " + tr("Stream size:") + " " + MediaItem::formatBytes(a.size) + "<br />\r\n";
            retval += " &nbsp; &nbsp; " + tr("Bitrate:") + " " + MediaItem::formatBits(a.bitRate) + "/s " + a.bitRateMode + "<br />\r\n";
            retval += " &nbsp; &nbsp; " + tr("Sampling rate:") + " " + MediaItem::formatRate(a.samplingRate) + "<br />\r\n";
            retval += " &nbsp; &nbsp; " + tr("Channels:") + QString(" %1<br />\r\n").arg(a.channels);
        }
    }

    retval += "<br /><br />\r\n\r\n";

    return retval;
}

void VideoItem::getData(Data &d){
    if(data->hasChildNodes() && data->firstChild().hasChildNodes())
    {
        QDomNodeList list = data->firstChild().childNodes();
        for(int i = 0; i < list.count(); i++){
            if(list.at(i).attributes().namedItem("type").toAttr().value() == "Video"){
                QDomNodeList v = list.at(i).childNodes();

                for(int j = 0; j < v.count(); j++){
                    QDomElement element = v.at(j).toElement();

                    if(element.nodeName() == "Duration"){
                        if (d.video.duration == 0)
                        {
                            d.video.duration = element.text().toInt();
                        }
                    }
                    else if(element.nodeName() == "Codec_ID_Info" || element.nodeName() == "Codec_description" ||
                            element.nodeName() == "Codec_ID" || element.nodeName() == "Codec"){
                        if (d.video.videoCodec == "")
                        {
                            d.video.videoCodec = element.text();
                        }
                    }
                    else if(element.nodeName() == "Width"){
                        if (d.video.width == 0)
                        {
                            d.video.width = element.text().toInt();
                        }
                    }
                    else if(element.nodeName() == "Height"){
                        if (d.video.height == 0)
                        {
                            d.video.height = element.text().toInt();
                        }
                    }
                    else if(element.nodeName() == "Bit_rate"){
                        if (d.video.bitRate == 0)
                        {
                            d.video.bitRate = element.text().toLong();
                        }
                    }
                    else if(element.nodeName() == "Bit_rate_mode"){
                        if (d.video.bitRateMode == "")
                        {
                            d.video.bitRateMode = element.text();
                        }
                    }
                    else if(element.nodeName() == "Frame_rate"){
                        if (d.video.framePerSec == 0)
                        {
                            d.video.framePerSec = element.text().toFloat();
                        }
                    }
                    else if(element.nodeName() == "Display_aspect_ratio"){
                        if (d.video.ratio == "")
                        {
                            d.video.ratio = element.text();
                        }
                    }
                    else if(element.nodeName() == "Stream_size"){
                        if (d.video.size == 0)
                        {
                            d.video.size = element.text().toLong();
                        }
                    }
                }
            }
            else if(list.at(i).attributes().namedItem("type").toAttr().value() == "Audio"){
                QDomNodeList a = list.at(i).childNodes();
                Data::Audio audio;

                for(int j = 0; j < a.count(); j++){
                    QDomElement element = a.at(j).toElement();

                    if(element.nodeName() == "Codec_Info"){
                        if (audio.audioCodec == "")
                        {
                            audio.audioCodec = element.text();
                        }
                    }
                    else if(element.nodeName() == "Channel_s_"){
                        if (audio.channels == 0)
                        {
                            audio.channels = element.text().toInt();
                        }
                    }
                    else if(element.nodeName() == "Channel_count"){
                        if (audio.channels == 0)
                        {
                            audio.channels = element.text().toInt();
                        }
                    }
                    else if(element.nodeName() == "Bit_rate"){
                        if (audio.bitRate == 0)
                        {
                            audio.bitRate = element.text().toInt();
                        }
                    }
                    else if(element.nodeName() == "Bit_rate_mode"){
                        if (audio.bitRateMode == "")
                        {
                            audio.bitRateMode = element.text();
                        }
                    }
                    else if(element.nodeName() == "Sampling_rate"){
                        if (audio.samplingRate == 0)
                        {
                            audio.samplingRate = element.text().toInt();
                        }
                    }
                    else if(element.nodeName() == "Stream_size"){
                        if (audio.size == 0)
                        {
                            audio.size = element.text().toLong();
                        }
                    }
                    else if(element.nodeName() == "Duration"){
                        if (audio.duration == 0)
                        {
                            audio.duration = element.text().toInt();
                        }
                    }
                }

                d.audios.append(audio);
            }
            else if(list.at(i).attributes().namedItem("type").toAttr().value() == "General"){
                QDomNodeList g = list.at(i).childNodes();

                for(int j = 0; j < g.count(); j++){
                    QDomElement element = g.at(j).toElement();

                    if(g.at(j).nodeName() == "File_size"){
                        if (d.size == 0)
                        {
                            d.size = element.text().toLong();
                        }
                    }
                    else if(g.at(j).nodeName() == "Codec"){
                        if (d.codec == "")
                        {
                            d.codec = element.text();
                        }
                    }
                    else if(g.at(j).nodeName() == "Codec_Info"){
                        if (d.codecInfo == "")
                        {
                            d.codecInfo = element.text();
                        }
                    }
                }
            }
        }
    }
}

VideoItem::Data::Data(){
    size = 0;
}

VideoItem::Data::Video::Video(){
    duration = bitRate = size = width = height = framePerSec = 0;
}

VideoItem::Data::Audio::Audio(){
    duration = bitRate = samplingRate = channels = size = 0;
}
