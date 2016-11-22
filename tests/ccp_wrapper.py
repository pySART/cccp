#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections import namedtuple
import ctypes
from ctypes import Structure, Union, c_uint8, c_uint16, c_int32, c_uint32, c_void_p, c_float, c_char_p
from ctypes import create_string_buffer, POINTER, byref, CFUNCTYPE, pointer, ARRAY, c_bool, byref
import enum

from ffi import FFI


CCP_LSDU_LEN = 8

class Ccp_MessageObjectType(Structure):

    _pack_ = 1
    _fields = [
        ("canID", c_uint32),
        ("dlc", c_uint8),
        ("data", c_uint8 * CCP_LSDU_LEN),
    ]


class Ccp(FFI):

    _functions_ = (
        ("Ccp_GetConnectionState", c_uint16, []),
        ("Ccp_DispatchCommand", POINTER(Ccp_MessageObjectType), []),
        ("Ccp_Init", None, []),
        ("Ccp_GetMta", c_uint32, []),
    )

    def init(self):
        self.Ccp_Init()

    def deinit(self):
        pass

    def getConnectionState(self):
        return self.Ccp_GetConnectionState

    def getMta(self):
        return self.Ccp_GetMta()

    def dispatchCommand(self, command):
        self.Ccp_DispatchCommand(byref(command))


class LocalTransport(object):
    pass

# Skip MacOSX tests for now.
