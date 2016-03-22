# gnuFile.py
__id__       = "$Id: gnuFile.py 632 2009-06-23 21:04:48Z jlconlin $"
__author__   = "$Author: jlconlin $"
__version__ = " $Revision: 632 $"
__date__     = "$Date: 2009-06-23 15:04:48 -0600 (Tue, 23 Jun 2009) $"


import sys
import numpy
import re
import os
import time
import string
import wx

import matplotlib
matplotlib.use('WXAgg')
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigCanvas, \
    NavigationToolbar2WxAgg as NavigationToolbar
import matplotlib.pyplot as pyplot

class Write_gnuFile(object):
    """
    Write_gnuFile will take data and write it to a file that should be readable
    by the gnuFile class.  I know this is written in C++, but I need a Python
    version as well.  Perhaps I should wrap it?  Nah.
    """
    def __init__ (self, filename, data, header=None):
        """
        filename:  Name of file you want to write to
        data: dictionary of data you want written.  dictionary values must be
            a tuple of x and y values.
        header: String of extra information to add to Header section
        """
        self.data = data
        if not header:
            header = ''
        self._prepareLines(header)
        self._writeFile(filename)
        object.__init__(self)


    def _prepareLines(self, header):
        """
        _prepareLines will prepare the lines which will be written to the file.

        data: Data that will be written to a file.
        header: String of extra information to add to Header section
        """
        self.Lines = []
        Header = """# This file was created by gnuFile.py version %s on %s.  
# This file is written with the intent of being used to plot the data with
# Gnuplot.  It contains no information regarding how the plot should be
# formatted.
# =============================================================================
#
# %s$ %s
# %s
# =============================================================================
# """  %(__version__, 
        time.asctime(), os.uname()[1], string.join(sys.argv), header)
        self.Lines.append(Header)
        
        if not isinstance(self.data, dict):
            raise TypeError, "Data must be in dictionary type."
        
        keys = self.data.keys()
        keys.sort(cmp=self.sortKeys)
        datalines = []
        nline = '# '
        tline = '# '
        cline = '#  '
        counter = 1

        # Prepare Column Headers
        for key in keys:
            if len(self.data[key]) == 3:
                nline = nline + ' %-13i%-13i%-13i' %(counter, counter+1,
                        counter+2)
                tline = tline + "|-%s-| " %string.center(key, 35)
                cline = cline + '%-13s%-13s%-13s ' %('x-coord', 'y-coord', 'error')
                counter +=3
            elif len(self.data[key]) == 2:
                nline = nline + ' %-13i%-13i' %(counter, counter+1)
                tline = tline + "|-%s-| " %string.center(key, 22)
                cline = cline + '%-13s%-13s ' %('x-coord', 'y-coord')
                counter +=2
            else:
                raise TypeError("Data must be tuples of length 2 or 3.")

        # Prepare Data lines
        for i in xrange(self.Longest()):
            line = "  "
            for key in keys:
                if len(self.data[key][0]) > i:
                    if len(self.data[key]) == 3:
                        line += ' %-12.5G %-12.5G %-12.5G ' %(
                                self.data[key][0][i], self.data[key][1][i], self.data[key][2][i])
                    elif len(self.data[key]) == 2:
                        line += ' %-12.5G %-12.5G ' %(
                                self.data[key][0][i], self.data[key][1][i])
                else:       # Data list doesn't contain this many elements
                    line += ' '*(12*len(self.data[key]) + len(self.data[key]) + 1)
            datalines.append(line)


        self.Lines.append(nline)
        self.Lines.append(tline)
        self.Lines.append(cline)
        self.Lines.extend(datalines)

    
    def sortKeys(self, k1, k2):
        """
        sortKeys is used to sort the data elements.
        """
        if len(self.data[k1][0]) < len(self.data[k2][0]):
            return 1
        elif len(self.data[k2][0]) < len(self.data[k1][0]):
            return -1
        else:
            return 0

    def Longest(self):
        """
        Longest will determine which data element has the largest length.  It
        returns this length.
        """
        longest = 0
        for key in self.data:
            if len(self.data[key][0]) > longest:
                longest = len(self.data[key][0])
        return longest
    
    def _writeFile(self, filename):
        """
        _writeFile will do the actual writing of the data to the file.

        filename: Name of file to be written.
        """
        outfile = open(filename, 'w')
        for line in self.Lines:
            outfile.write(line + '\n')
        outfile.close()


class gnuFile(object):
    """
    gnuFile will read a file generated by my gnuFile class (written in C++). 
    It will parse the columns and store them in a dictionary.
    """

    def __init__(self, filename):
        object.__init__(self)

        self.f = open(filename, 'r')
        self.Data = {}

        self._Parse()

    def _Parse(self):
        """
        Parse will look in self.f and parse the data to get the columns.
        """
        header = re.compile(r'''
                (\|\-                    # Open pipe
                \s*                      # Some whitespace
                ([\S|\s]+?)                    # Data label
                \s*                      # Some whitespace
                \-\|)                    # Close pipe
                ''', re.VERBOSE)

        # Read until we have header line
        for line in self.f:
            headers = header.findall(line)
            if headers:    # Found headerline
                break
        
        for line in self.f:     # Next line is data label line
            words = line.split()
            break   # Only read one line

        # Process data labels
        words.pop(0)
        h = 0   # Which header are we working with
        while words:    # Only do this while we have data labels to seperate
            if len(words) > 2 and words[2][0] == 'e':   # Have errorbar data
                self.Data[headers[h][1]] = [[],[],[]]
                for i in xrange(3):
                    words.pop(0)
            else:                   # Don't have errobar data, just x,y
                self.Data[headers[h][1]] = [[],[]]
                for i in xrange(2):
                    words.pop(0)
            h +=1

        for line in self.f:     # Continue reading the rest of the file 
            words = line.split()
            w = 0       # Which words is the current word
            for i in xrange(len(headers)):
                key = headers[i][1]
                for j in xrange(len(self.Data[key])):
                    self.Data[key][j].append(float(words[w]))
                    w +=1
                if w >= len(words): break

        for key in self.Data.keys():
            self.Data[key] = numpy.transpose(numpy.array(self.Data[key]))

class AnimateDialog(wx.Dialog):
    def __init__(self, parent, ID, title, size = wx.DefaultSize, 
            pos=wx.DefaultPosition, style = wx.DEFAULT_DIALOG_STYLE ):

        wx.Dialog.__init__(self, parent, ID, title, pos=pos, size=size, 
                style=style)

        self.mainSizer = wx.BoxSizer(wx.VERTICAL)
        fgSizer = wx.FlexGridSizer(rows=2, cols=2, vgap=20, hgap=10)

        # Set up widgets
        self.filename = "image.png"
        self.directory = os.path.join(os.getcwd(), "tmp")
        filenameText = wx.StaticText(self, -1, "Filename:")

        self.filenameCtrl = wx.TextCtrl(self, -1, self.filename)
        dText = wx.StaticText(self, -1, "Directory:")
        self.directoryCtrl = wx.TextCtrl(self, -1, self.directory, 
                 size=(300,60), style=wx.TE_MULTILINE)

        directoryButton = wx.Button(self, -1, "Change Directory")
        self.Bind(wx.EVT_BUTTON, self.ChangeDirectory, directoryButton)

        Explanation = "Each plot will be saved of the form nameXXX.ext where\n"
        Explanation += "name is the filename (image), ext is the file extension\n"
        Explanation += "(png), and XXX is a unique integer starting at 000."
        explainText = wx.StaticText(self, -1, Explanation, style=wx.ALIGN_LEFT)

        line = wx.StaticLine(self, -1, size=(20,-1), style=wx.LI_HORIZONTAL)

        # Add widgets to sizer
        fgSizer.Add(filenameText, proportion=0, flag=0)
        fgSizer.Add(self.filenameCtrl, proportion=0, flag=0)
        fgSizer.Add(dText, proportion=0, flag=0)
        fgSizer.Add(self.directoryCtrl, proportion=0, flag=wx.EXPAND)

        # Add sizer to self.mainSizer
        self.mainSizer.Add(fgSizer, proportion=0, flag=wx.EXPAND|wx.ALL, border=5)

        self.mainSizer.Add(directoryButton, proportion=0, flag=0)
        self.mainSizer.Add(line, 0, 
                wx.GROW|wx.ALIGN_CENTER_VERTICAL|wx.RIGHT|wx.TOP, 5)
        self.mainSizer.Add(explainText, proportion=0, flag=wx.EXPAND|wx.ALL, 
                border=5)
        self.mainSizer.Add(line, 0, 
                wx.GROW|wx.ALIGN_CENTER_VERTICAL|wx.RIGHT|wx.TOP, 5)

        self._OKCancel()

        self.SetSizer(self.mainSizer)

    def _OKCancel(self):
        """
        _OKCancel will set up the OK and Cancel buttons
        """
        ButtonSizer = wx.StdDialogButtonSizer()

        self.OKButton = wx.Button(self, wx.ID_OK)
        self.OKButton.SetDefault()
        ButtonSizer.AddButton(self.OKButton)

        self.CancelButton = wx.Button(self, wx.ID_CANCEL)
        ButtonSizer.AddButton(self.CancelButton)

        ButtonSizer.Realize()

        self.mainSizer.Add(ButtonSizer, 0, wx.ALIGN_CENTER_VERTICAL|wx.EXPAND)

    def ChangeDirectory(self, event):
        dialog = wx.DirDialog(self, "Choose a directory:", 
                defaultPath=os.getcwd() )

        if dialog.ShowModal() == wx.ID_OK:
            self.directory = dialog.GetPath()
            self.directoryCtrl.SetValue(self.directory)
        dialog.Destroy()

class MainFrame(wx.Frame):
    """
    Main window for editing stuff
    """
    def __init__(self, parent, ID, title, pos=wx.DefaultPosition, 
            size = wx.DefaultSize, style=wx.DEFAULT_FRAME_STYLE):
        wx.Frame.__init__(self, parent, ID, title, pos, size, style)

        self.mainSizer = wx.BoxSizer(wx.VERTICAL)

        self._CreateMenuBar()
        self._MakeStatusBar()
        
        # Filename information
        fileSizer = wx.BoxSizer(wx.HORIZONTAL)
        self.filename = 'data.dat'
        f1 = wx.StaticText(self, -1, "Filename:", pos=(10,10) )
        self.fileText = wx.StaticText(self, -1, self.filename, size = (500, 20))
        fileSizer.Add(f1, 0, wx.LEFT|wx.RIGHT|wx.EXPAND, border=5)
        fileSizer.Add(self.fileText, 0, wx.LEFT|wx.RIGHT|wx.ALIGN_BOTTOM, 
                border=5)
        self.mainSizer.Add(fileSizer, 0, wx.EXPAND|wx.ALL, border=5)

        # Regular expression
        regExSizer = wx.BoxSizer(wx.HORIZONTAL)
        l1 = wx.StaticText(self, -1, "Regular Expression:")
        self.rxCtrl = wx.TextCtrl(self, -1, "(\d+)-Real", size = (200,20))
        self.rxButton = wx.Button(self, -1, "Parse")
        self.rxButton.SetDefault()
        self.Bind(wx.EVT_BUTTON, self.RegExChange, self.rxButton)
        regExSizer.Add(l1, 0, wx.ALL|wx.EXPAND, border=5)
        regExSizer.Add(self.rxCtrl, 0, wx.LEFT|wx.RIGHT|
                wx.ALIGN_CENTER_VERTICAL, border=5)
        regExSizer.Add(self.rxButton, 0, wx.LEFT|wx.RIGHT|
                wx.ALIGN_CENTER_VERTICAL, border=5)
        self.mainSizer.Add(regExSizer, 0, wx.ALL|wx.EXPAND, border=5)
        self.mainSizer.Add(wx.StaticLine(self, -1, style=wx.LI_HORIZONTAL), 0, 
                wx.ALL|wx.EXPAND, border=5)

        self._SetupKeyStuff()
        self.SetSizer(self.mainSizer)
        self.mainPanel = wx.Panel(self, -1)
        self.Fit()      # Fit frame/window around children

    def _SetupKeyStuff(self):
        self.DataKeys = []
        self.RegExKeys = []
        self.CheckedKeys = []

        # Set up HORIZONTAL sizer to help place items
        self.midSizer = wx.BoxSizer(wx.HORIZONTAL)
        keySizer = wx.BoxSizer(wx.VERTICAL)
#       regExKeySizer = wx.BoxSizer(wx.VERTICAL)

        # Regular Expression results
        regExKeyText = wx.StaticText(self, -1, "RegEx keys:")
        self.regExCountText = wx.StaticText(self, 01, "")
        self.lb = wx.CheckListBox(self, -1, choices=[], size=(250,250), 
                style=wx.LB_EXTENDED )
        self.Bind(wx.EVT_CHECKLISTBOX, self.EvtCheckListBox, self.lb)
        self.Bind(wx.EVT_LISTBOX, self.EvtListBox, self.lb)

        keySizer.Add(regExKeyText, 0, wx.LEFT|wx.RIGHT, border=5)
        keySizer.Add(self.lb, 0, wx.ALL, border=5)
        keySizer.Add(self.regExCountText, 0, wx.TOP|wx.BOTTOM|wx.ALIGN_LEFT, 
                border=5)

        # ListBox buttons
        ClearButton = wx.Button(self, -1, "Clear", pos=(350, 405))
        AllButton = wx.Button(self, -1, "All", pos=(450, 405))
        ToggleButton = wx.Button(self, -1, "Toggle", pos=(550, 405))
        self.Bind(wx.EVT_BUTTON, self.ClearSelection, ClearButton)
        self.Bind(wx.EVT_BUTTON, self.SelectAll, AllButton)
        self.Bind(wx.EVT_BUTTON, self.ToggleSelection, ToggleButton)

        lbSizer = wx.BoxSizer(wx.HORIZONTAL)
        lbSizer.Add(ClearButton, 0, wx.LEFT|wx.RIGHT, border=5)
        lbSizer.Add(AllButton, 0, wx.LEFT|wx.RIGHT, border=5)
        lbSizer.Add(ToggleButton, 0, wx.LEFT|wx.RIGHT, border=5)
        keySizer.Add(lbSizer, 0, wx.LEFT|wx.RIGHT|wx.EXPAND, border=5)

        # Animate
        animateSizer = wx.BoxSizer(wx.HORIZONTAL)
        AnimateButton = wx.Button(self, -1, "Animate")
        self.animateGauge = wx.Gauge(self, -1, 50)
        self.Bind(wx.EVT_BUTTON, self._AnimateEvent, AnimateButton)
        animateSizer.Add(AnimateButton, 1, wx.ALL, border=5)
        animateSizer.Add(self.animateGauge, 2, wx.ALL|wx.EXPAND, border=5)

        keySizer.Add(animateSizer, 0, wx.ALL, border=5)

        # Available keys
        dataKeyText = wx.StaticText(self, -1, "Data keys:")
        self.DataKeysCtrl = wx.TextCtrl(self, -1, '',size=(250,200),
                style=wx.TE_MULTILINE|wx.TE_READONLY)
        self.Bind(wx.EVT_TEXT, self.RegExChange, self.DataKeysCtrl)
        keySizer.Add(dataKeyText, 0, wx.LEFT|wx.RIGHT|wx.EXPAND, border=5)
        keySizer.Add(self.DataKeysCtrl, 0, wx.ALL, border=5)
        self.midSizer.Add(keySizer, 1, wx.ALL, border=5)
        
        self.midSizer.Add(wx.StaticLine(self, -1, style=wx.LI_VERTICAL), 0, 
                wx.ALL|wx.EXPAND, border=5)

        # matplotlib stuff
        self._SetupMPL()
        self.mainSizer.Add(self.midSizer, 0, wx.ALL|wx.EXPAND, border=5)

    def _SetupMPL(self):
        self.dpi = 100
        self.fig = Figure( (6.5, 4.5), dpi = self.dpi, edgecolor='black' )
        self.canvas = FigCanvas(self, -1, self.fig)
        self.axes = self.fig.add_subplot(111)
        self.axes.set_axis_bgcolor('white')
        self.mplToolbar = NavigationToolbar(self.canvas)

        # Layout with sizers
        mplBox = wx.BoxSizer(wx.VERTICAL)
        mplBox.Add(self.canvas, 0, wx.LEFT|wx.TOP|wx.EXPAND)
        mplBox.Add(self.mplToolbar, 0, wx.LEFT|wx.TOP|wx.EXPAND)
        mplBox.Add((10,10))

        plotOptions = wx.BoxSizer(wx.HORIZONTAL)

        # Axes labels
        labelBox = wx.StaticBox(self, -1, "Axes Labels")
        labelSizer = wx.StaticBoxSizer(labelBox, wx.VERTICAL)
        xLabelText = wx.StaticText(self, -1, "x Axis:")
        yLabelText = wx.StaticText(self, -1, "y Axis:")
        self.xLabelCtrl = wx.TextCtrl(self, -1, "slab width (mfp)", 
                size=(150,20) )
        self.yLabelCtrl = wx.TextCtrl(self, -1, "fission source", 
                size=(150,20) )
        self.axes.set_xlabel(self.xLabelCtrl.GetValue())
        self.axes.set_ylabel(self.yLabelCtrl.GetValue())

        self.Bind(wx.EVT_TEXT_ENTER, self.xLabel, self.xLabelCtrl)
        self.Bind(wx.EVT_TEXT_ENTER, self.yLabel, self.yLabelCtrl)

        labelSizer.Add(xLabelText, 0, wx.ALL, border=5)
        labelSizer.Add(self.xLabelCtrl, 0, wx.ALL, border=5)
        labelSizer.Add(yLabelText, 0, wx.ALL, border=5)
        labelSizer.Add(self.yLabelCtrl, 0, wx.ALL, border=5)
        plotOptions.Add(labelSizer, wx.ALL, border=5)

        # Plot Style options
        styleBox = wx.StaticBox(self, -1, "Plot Styles")
        styleSizer = wx.StaticBoxSizer(styleBox, wx.VERTICAL)

        styleChoices = ['steps', 'errorbars', 'lines', 'linespoints', 'points']
        self.plotStyle = wx.Choice(self, -1, choices=styleChoices)
        styleSizer.Add(self.plotStyle, 0, wx.ALL, border=5)
        plotOptions.Add((20,20))
        plotOptions.Add(styleSizer, wx.ALL, border=5)

        # Plot title
        titleBox = wx.StaticBox(self, -1, "Plot title")
        titleSizer = wx.StaticBoxSizer(titleBox, wx.VERTICAL)

        explanation = "This title is ignored during animation."
        titleExplain = wx.StaticText(self, -1, explanation, style=wx.ALIGN_LEFT)
        self.titleCtrl = wx.TextCtrl(self, -1, "Fission Source Distribution")
        self.axes.set_title(self.titleCtrl.GetValue())

        titleSizer.Add(self.titleCtrl, 0, wx.ALL|wx.EXPAND, border=5)
        titleSizer.Add(titleExplain, 0, wx.ALL, border=5)
        plotOptions.Add((20,20))
        plotOptions.Add(titleSizer)

        mplBox.Add(plotOptions)
        self.midSizer.Add(mplBox, 2, wx.ALL, border=5)

    def _CreateMenuBar(self):
        """
        CreateMenuBar will create the menubar for the class
        """
        # Create the menubar
        mb = wx.MenuBar()

        # and a menu 
        file_menu = wx.Menu()

        # add an item to the menu, using \tKeyName automatically
        # creates an accelerator, the third param is some help text
        # that will show up in the statusbar
        file_menu.Append(wx.ID_OPEN, "&Open\tCtrl-O", "Open a file")
        file_menu.AppendSeparator()
        file_menu.Append(wx.ID_EXIT, "E&xit\tAlt-X", "Exit this simple sample")

        # bind the menu event to an event handler
        self.Bind(wx.EVT_MENU, self.OnExit, id=wx.ID_EXIT)
        self.Bind(wx.EVT_MENU, self.OnOpen, id=wx.ID_OPEN)

        # and put the menu on the menubar
        mb.Append(file_menu, "&File")
        self.SetMenuBar(mb)            # Add menuBar to Frame content

    def _MakeStatusBar(self):
        self.statusbar = self.CreateStatusBar(2, wx.ST_SIZEGRIP)
        self.statusbar.SetStatusWidths([-2, -1])
        statusbar_fields = [("gnuFile parser"), ("First attempte at a GUI")]
        for field, i in zip( statusbar_fields, xrange(len(statusbar_fields)) ):
            self.statusbar.SetStatusText(field, i)
        self.SetStatusBar(self.statusbar)

    def RegExChange(self, e):
        self.ParseRegExKeys()
        self.FillRegExKeys()

    def OnCloseWindow(self, event):
        self.Destroy()

    def OnExit(self, e):
        self.Close(True)    # Close the frame

    def OnOpen(self, e):
        """
        Open a file
        """
        self.dirname = os.getcwd()
        dialog = wx.FileDialog(self, "Choose a file", self.dirname, "", "*.*",
                wx.OPEN)
        if dialog.ShowModal() == wx.ID_OK:
            self.filename = dialog.GetFilename()
            self.dirname = dialog.GetDirectory()

            try:
                self.filename = os.path.join(self.dirname, self.filename)
                self.gF = gnuFile(self.filename)
                # Display the file in control box
                self.fileText.SetLabel(self.filename)
                self.InitialFill()
            except: 
                message = "Error opening that file.  Please choose another."
                self.fileText.SetLabel(message)

        dialog.Destroy()
        
    def InitialFill(self):
        """
        InitialFill is called when a file is first opened.
        """
        self.DataKeys = self.gF.Data.keys()
        self.DataKeys.sort()
        data = ""
        for key in self.DataKeys:
            data += "%s\n" %key
        self.DataKeysCtrl.SetValue(data)
        self.ParseRegExKeys()
        self.FillRegExKeys()

    def ParseRegExKeys(self):
        """
        ParseRegExKeys will parse self.RegExKeys
        """
        self.RegExKeys = []
        reg = self.rxCtrl.GetValue()
        r = re.compile(reg)
        for i, key in zip( xrange( len(self.DataKeys) ), self.DataKeys ):
            valid = r.search(key)
            if valid: 
                if valid.groups():   # groups specified on regular expression
                    self.RegExKeys.append( (int(valid.group(1)), key ) )
                else:               # No groups were specified on regular expression
                    self.RegExKeys.append( (0, key ) )

    def FillRegExKeys(self):
        """
        FillRegExKeyswill fill the RegExKeys control with the keys selected by
        the regular expression
        """
        data = ""
        boxData = []
        self.RegExKeys.sort()
        self.lb.Clear()
        for key in self.RegExKeys: 
            data += "%s\n" %key[1]
            self.lb.InsertItems([key[1]], 0)
        self.regExCountText.SetLabel("Number of matching keys: %s" %len(self.RegExKeys))

    def EvtListBox(self, event):
        """
        EvtListBox is called whenever an item in self.lb is selected
        """
        indexs = self.lb.GetSelections()
        self.axes.clear()
        
        for index in indexs:
            key = self.lb.GetString(index)
            self.Plot(key)

    def EvtCheckListBox(self, event):
        """
        EvtCheckListBox is called whenever an item in self.lb is checked
        """
        index = event.GetSelection()
        key = self.lb.GetString(index)

        if self.lb.IsChecked(index):
            self._AddtoChecked(index, key)
        else: 
            self._RemovefromChecked(index, key)

    def xLabel(self, event):
        self.axes.xlabel = self.xLabelCtrl.GetValue()
        self.axes.set_xlabel(self.xLabelCtrl.GetValue())

    def yLabel(self, event):
        self.axes.ylabel = self.yLabelCtrl.GetValue()
        self.axes.set_ylabel(self.yLabelCtrl.GetValue())

    def _AddtoChecked(self, index, key):
            # Add tuple to list
            self.CheckedKeys.append( (index, key) )

    def _RemovefromChecked(self, index, key):
            # Remove tuple from list
            self.CheckedKeys.pop( self.CheckedKeys.index( (index,key) ) )

    def ClearSelection(self, e):
        self.CheckedKeys = []
        for n in xrange(self.lb.GetCount()): self.lb.Check(n, check=False)

    def SelectAll(self, e):
        self.CheckedKeys = self.RegExKeys
        for n in xrange(self.lb.GetCount()): self.lb.Check(n, check=True)

    def ToggleSelection(self, e):
        for n in xrange( self.lb.GetCount() ):
            if self.lb.IsChecked(n): 
                check = False
                self._RemovefromChecked( n, self.lb.GetString(n) )
            else: 
                check = True
                self._AddtoChecked( n, self.lb.GetString(n) )
            self.lb.Check(n, check=check)
    
    def _AnimateEvent(self, event):

        self.CheckedKeys.sort()
        AD = AnimateDialog(self, -1, "Animate")
        AD.Centre()

        val = AD.ShowModal()
        if val == wx.ID_OK:
            self.Animate(AD.filename, AD.directory)
        AD.Destroy()

    def Plot(self, key, title=None):
        """
        Plot will plot the data from self.gF.Data[key] in self.canvas
        """
        x = numpy.transpose( self.gF.Data[key] )
        index = self.plotStyle.GetCurrentSelection()
        style = self.plotStyle.GetString(index)
        
        if style == 'steps':
            self.axes.plot(x[0], x[1], ls='steps')
        elif style == 'errorbars':
            try:
                self.axes.errorbar(x[0],x[1],x[2])
            except IndexError:
                print "You must have errorbar data in your input file"
        elif style ==  'lines':
            self.axes.plot(x[0],x[1], '-')
        elif style == 'linespoints':
            self.axes.plot(x[0],x[1], '.-')
        elif style == 'points':
            self.axes.plot(x[0],x[1], '.')
        else: raise KeyError, "I don't know how to plot that style, yet"

        if not title:   
            self.axes.set_title(self.titleCtrl.GetValue())
        self.axes.set_xlabel(self.xLabelCtrl.GetValue())
        self.axes.set_ylabel(self.yLabelCtrl.GetValue())
        self.canvas.draw()

    def Animate(self, filename, directory):
        """
        Animate will create the sequence of images using self.CheckedKeys to 
        know which items to plot.
        """
        root, ext = os.path.splitext(filename)
        range = len(self.CheckedKeys)
        self.animateGauge.SetRange(range)
        for key, i in zip(self.CheckedKeys, xrange(range) ):
            self.axes.clear()
            self.Plot(key[1], key[1])
            fName = os.path.join(directory, "%s%03i%s" %(root, i, ext) ) 
            self.canvas.print_png( fName )

class MyApp(wx.App):
    def OnInit(self):
        frame = MainFrame(None, -1, 'gnuFile parser', size=(1000,700))
        self.SetTopWindow(frame)
        frame.Show(True)
        return True
        

if __name__ == "__main__":
    app = MyApp(redirect=False)
    app.MainLoop()

