#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections import namedtuple
import ctypes
from ctypes import Structure, Union, c_uint8, c_uint16, c_int32, c_uint32, c_void_p, c_float, c_char_p
from ctypes import create_string_buffer, POINTER, byref, CFUNCTYPE, pointer, ARRAY, c_bool
import enum

from ffi import FFI

class KnxMsg_StandardFrameType(Structure):
    _fields_ = [
        ("ctrl", c_uint8),
        ("source", c_uint8 * 2),
        ("dest", c_uint8 * 2),
        ("npci", c_uint8),
        ("tpci", c_uint8),
        ("apci", c_uint8),
        #("data", c_uint8 * MAX_APDU_LEN),
    ]
    _pack_ = 1

class Ccp(FFI):

    _functions_ = (
        ("Ccp_GetConnectionState", c_uint16, []),
        ("Ccp_DispatchCommand", None, []),
        ("Ccp_Init", None, []),
#        ("Utl_BitGet", c_bool, [c_uint16, c_uint8]),
#        ("Utl_BitSet", c_uint16, [c_uint16, c_uint8]),
#        ("Utl_BitReset", c_uint16, [c_uint16, c_uint8]),
#        ("Utl_BitToggle", c_uint16, [c_uint16, c_uint8]),
#        ("Utl_BitGetHighest", c_uint16, [c_uint16]),
#        ("Utl_BitGetLowest", c_uint16, [c_uint16]),
#        ("Utl_BitSetLowest", c_uint16, [c_uint16]),
#        ("Utl_BitResetLowest", c_uint16, [c_uint16]),
#        ("Utl_Log2", c_uint8, [c_uint16]),
#        ("Utl_Sqrt16", c_uint16, [c_uint16]),
#        ("Utl_Sqrt32", c_uint32, [c_uint32]),
#        ("Utl_Itoa", None, [c_uint32, c_uint8, c_char_p]),
#        ("Utl_MemCopy", None, [c_void_p, c_void_p, c_uint16]),
#        ("Utl_MemSet", None, [c_void_p, c_uint8, c_uint16]),
#        ("Utl_StrCat", None, [c_void_p, c_void_p]),
    )

    def deinit(self):
        pass

    def checkEndianess(self):
        return Utl_EndianessType(self.Utl_CheckEndianess)

    def bitGet(self, w, num):
        return self.Utl_BitGet(w, num)

    def bitSet(self, w, num):
        return self.Utl_BitSet(w, num)

    def bitReset(self, w, num):
        return self.Utl_BitReset(w, num)

    def bitToggle(self, w, num):
        return self.Utl_BitToggle(w, num)

    def bitGetHighest(self, w):
        return self.Utl_BitGetHighest(w)

    def bitGetLowest(self, w):
        return self.Utl_BitGetLowest(w)

    def bitSetLowest(self, w):
        return self.Utl_BitSetLowest(w)

    def bitResetLowest(self, w):
        return self.Utl_BitResetLowest(w)

    def log2(self, w):
        return self.Utl_Log2(w)

    def sqrt16(self, w):
        return self.Utl_Sqrt16(w)

    def sqrt32(self, w):
        return self.Utl_Sqrt32(w)

    def itoa(self, value, base):
        buf = create_string_buffer(32)
        self.Utl_Itoa(value, base, buf)
        return buf.value

    def memCopy(self, dst, src, length):
        self.Utl_MemCopy(dst, src, length)

    def memSet(self, dst, ch, length):
        self.Utl_MemSet(dst, ch, length)

    def strCat(self, dst, src):
        self.Utl_StrCat(dst, src)

    def strCpy(self, dst, src):
        self.Utl_StrCpy(dst, src)

#    def strNCpy(self, dst, src, length):
#        self.Utl_StrNCpy(dst, src, length)

    def strRev(self, dst):
        self.Utl_StrRev(dst)

    def strChr(self, dst, ch):
        return self.Utl_StrChr(dst, ch)

    def strLen(self, dst):
        return self.Utl_StrLen(dst)

    def ntohs(self, value):
        return self.Utl_Ntohs(value)

    def htons(self, value):
        return self.Utl_Htons(value)

#    def divrem(self, l, r):
#        divrem = Utl_DivremType()
#        self.Utl_Divrem(l, r, byref(divrem))
#        return divrem

    def floatsAreNearlyEqual(self, a, b, diff):
        return self.Utl_FloatsAreNearlyEqual(a, b, diff)

    def randomize(self, seed):
        self.Utl_Randomize(seed)

    def random(self):
        return self.Utl_Random()

