#! /usr/bin/env python

from lxml import etree 
import xlrd             
import rospy
from pykml.factory import KML_ElementMaker as KML 
import csv # csv reading needed library
import pandas as pd
from novatel_msgs.msg import BESTPOS
from sensor_msgs.msg   import NavSatFix


class pullh2kml_eval():
    def __init__(self):
        rospy.Subscriber('/nmea_trajectory', NavSatFix, self.nmea_callback)
        self.lat_ = [] # used to save latitude
        self.lon_ = [] # used to save longitude
        self.alt_ = [] # used to save altitude

    def nmea_callback(self,data):
        self.longitude = data.longitude
        self.latitude  = data.latitude
        self.altitude  = data.altitude
        self.lat_.append(float(self.latitude))
        self.lon_.append(float(self.longitude))
        self.alt_.append(float(self.altitude))
        print'latitude = ', self.latitude
        print'longitude = ', self.longitude
        print'altitude = ', self.altitude
        print'--------------------------------'

        fold = KML.Folder(KML.Placemark(KML.Point(KML.coordinates(str(self.lon_[0]) + ',' + str(self.lat_[0]) + ',0'))))
        for i in range(1, len(self.lon_)):
            fold.append(KML.Placemark(
                KML.Point(KML.coordinates(str(self.lon_[i]) + ',' + str(self.lat_[i]) + ',0'))))
        content = etree.tostring(etree.ElementTree(fold), pretty_print=True)
        with open('nmea_trajectory.kml', 'w') as fp:
            fp.write(content)

if __name__ == '__main__':
    rospy.init_node('pullh2kml_evaluGt', anonymous=True)
    pullh2kml_eval_ =pullh2kml_eval()
    rate = rospy.Rate(0.002)#
    preTim = 0.0
    while not rospy.is_shutdown():
        rate.sleep()
        print 'sleep end '