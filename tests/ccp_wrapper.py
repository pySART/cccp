#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections import namedtuple
import ctypes
from ctypes import Structure, Union, c_uint8, c_uint16, c_int32, c_uint32, c_void_p, c_float, c_char_p
from ctypes import create_string_buffer, POINTER, byref, CFUNCTYPE, pointer, ARRAY, c_bool, byref
import enum

from ffi import FFI

from pyccp import ccp


CCP_LSDU_LEN = 8

class Ccp_MessageObjectType(Structure):

    _pack_ = 1
    _fields = [
        ("canID", c_uint32),
        ("dlc", c_uint8),
        ("data", c_uint8 * CCP_LSDU_LEN),
    ]


SEND_FUNC = CFUNCTYPE(None, POINTER(Ccp_MessageObjectType))

def sender(cmo):
    print("CMO: {0}".format(cmo))

send_func = SEND_FUNC(sender)


class Ccp(FFI):

    _functions_ = (
        ("Ccp_GetConnectionState", c_uint16, []),
        ("Ccp_DispatchCommand", None, [POINTER(Ccp_MessageObjectType)]),
        ("Ccp_Init", None, []),
        ("Ccp_GetMta", c_uint32, []),
        ("Ccp_SetSendCallout", None, [SEND_FUNC]),
    )

    def init(self):
        self.Ccp_Init()

    def deinit(self):
        pass

    def getConnectionState(self):
        return self.Ccp_GetConnectionState()

    def getMta(self):
        return self.Ccp_GetMta()

    def setSendCallout(self, callout):
        self.Ccp_SetSendCallout(SEND_FUNC(callout))

    def dispatchCommand(self, command):
        self.Ccp_DispatchCommand(byref(command))


class LocalTransport(object):

    def __init__(self, dll):
        self.parent = None
        self.dll = dll

    def send(self, canID, b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0, b7 = 0):
        self.message = ccp.CANMessageObject(canID, 8, bytearray((b0, b1, b2, b3, b4, b5, b6, b7)))
#        print("[LocalTransport] Sending: {}".format(self.message))
        cmo = Ccp_MessageObjectType()
        cmo.canID = canID
        cmo.dlc = 8
        cmo.data = [b0, b1, b2, b3, b4, b5, b6, b7]
        #print(cmo.canID, cmo.dlc, cmo.data)
        self.dll.dispatchCommand(cmo)

    def receive(self, canID, b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0, b7 = 0):
        self.message = ccp.CANMessageObject(canID, 8, bytearray((b0, b1, b2, b3, b4, b5, b6, b7)))
        self.parent.receive(self.message)

    def __str__(self):
        return "[Current Message]: {}".format(self.message)

    __repr__ = __str__


##
##from ffi import loadLibrary
##
##dll = loadLibrary("ccp")
##print(dll)
##
##ccpDll = Ccp(dll)
##print(dir(ccpDll))
##ccpDll.init()
##print(ccpDll.getMta())
##

