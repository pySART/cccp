#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import sys
import enum

from ffi import loadLibrary


class BaseTest(unittest.TestCase):

    CLASS = None
    DLL = ""

    def setUp(self):
        self.obj = self.CLASS(loadLibrary(self.DLL))

    def tearDown(self):
        self.obj.deinit()
        del self.obj

