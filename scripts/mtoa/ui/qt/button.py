# Taken from renderSetup.views.renderSetupButton
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets

# from .Qt.QtCore import QSize, QEvent
# from .Qt.QtGui import QtGui.QColor, QtGui.QIcon, QtGui.QImage, QtGui.QPainter, QtGui.QPixmap, QtGui.qRgba, QtGui.qAlpha
# from .Qt.QtWidgets import QtWidgets.QAbstractButton, QtWidgets.QStyle, QtWidgets.QStyleOptionButton, QtWidgets.QToolTip

from .utils import dpiScale


class MtoAButton(QtWidgets.QAbstractButton):
    """
    This class represents a render setup button which is an icon button that produces a brighter version of the icon when hovered over.
    """

    # Constants
    DEFAULT_BUTTON_SIZE = dpiScale(20)
    HIGH_LIMIT = 205
    LOW_LIMIT = 30
    MAX_VALUE = 255
    ADJUSTMENT = MAX_VALUE - HIGH_LIMIT

    HIGHLIGHTED = 1
    DISABLED = 0

    def __init__(self, parent, icon, size=DEFAULT_BUTTON_SIZE, isEnabled=True):
        super(MtoAButton, self).__init__(parent)
        self.size = size
        self.enter = False
        # there is a pixmap to draw for mouseover events
        self.brighterPixmap = None
        # there is a pixmap to draw when the button is disabled
        self.disabledPixmap = None
        self.enabled = isEnabled
        self.iconChanged = False
        self.setIcon(icon)

    def setEnabled(self, enabled):
        self.enabled = enabled
        self.repaint()

    def isEnabled(self):
        return self.enabled

    def enterEvent(self, event):
        self.enter = True
        self.repaint()

    def leaveEvent(self, event):
        self.enter = False
        self.repaint()

    def setIcon(self, icon):
        super(MtoAButton, self).setIcon(icon)
        self.iconChanged = True

    def event(self, event):
        if event.type() == QtCore.QEvent.ToolTip:
            QtWidgets.QToolTip.showText(event.globalPos(), self.toolTip())
            return True
        if event.type() in [QtCore.QEvent.MouseButtonPress, QtCore.QEvent.MouseButtonDblClick] and not self.isEnabled():
            return False

        return super(QtWidgets.QAbstractButton, self).event(event)

    def createOption(self):
        option = QtWidgets.QStyleOptionButton()
        option.initFrom(self)
        if self.isDown():
            option.state = QtWidgets.QStyle.State_Sunken
        else:
            option.state = QtWidgets.QStyle.State_Raised

        if self.enter:
            option.state = option.state | QtWidgets.QStyle.State_MouseOver

        option.icon = self.icon()
        return option

    def paintEvent(self, e):
        """ Draws the render setup button """
        painter = QtGui.QPainter(self)
        option = self.createOption()
        self.drawControl(QtWidgets.QStyle.CE_PushButton, option, painter, self)

    def createPixmap(self, option):
        return option.icon.pixmap(self.size, self.size)

    def drawControl(self, element, option, painter, widget=None):
        """ Draws the icon button """
        pixmap = self.createPixmap(option)
        if not self.isEnabled():
            pixmap = self.getGeneratedIconPixmap(QtGui.QIcon.Disabled, pixmap, option)
        elif option.state & QtWidgets.QStyle.State_MouseOver:
            pixmap = self.getGeneratedIconPixmap(QtGui.QIcon.Active, pixmap, option)

        painter.drawPixmap(option.rect, pixmap, pixmap.rect())

    def generatePixmapActiveIcon(self, iconMode, pixmap, option):
        if self.brighterPixmap is None or self.iconChanged:
            self.brighterPixmap = self.generateHighlightedIconPixmap(pixmap)
            self.iconChanged = False
        return self.brighterPixmap

    def getGeneratedIconPixmap(self, iconMode, pixmap, option):
        """ Draws the icon button brighter when hovered over. """
        if iconMode == QtGui.QIcon.Normal:
            return pixmap
        elif iconMode == QtGui.QIcon.Active:
            return self.generatePixmapActiveIcon(iconMode, pixmap, option)
        elif iconMode == QtGui.QIcon.Disabled:
            if self.disabledPixmap is None or self.iconChanged:
                self.disabledPixmap = self.generateDisabledIconPixmap(pixmap)
                self.iconChanged = False
            return self.disabledPixmap

    def generateHighlightedIconPixmap(self, pixmap):
        return self.generateIconPixmap(pixmap, self.HIGHLIGHTED)

    def generateDisabledIconPixmap(self, pixmap):
        return self.generateIconPixmap(pixmap, self.DISABLED)

    # Code taken from QadskDarkStyle.cpp - QadskDarkStyle::generatedIconPixmap
    def generateIconPixmap(self, pixmap, type):
        img = pixmap.toImage().convertToFormat(QtGui.QImage.Format_ARGB32)
        imgh = img.height()
        imgw = img.width()
        for y in range(0, imgh):
            for x in range(0, imgw):
                pixel = img.pixel(x, y)
                color = QtGui.QColor(pixel)
                h, s, v, _ = color.getHsv()
                if v > self.LOW_LIMIT:  # value below this limit will not be adjusted
                    if v < self.HIGH_LIMIT:  # value above this limit will just max up to 255
                        v = v + self.ADJUSTMENT if type == self.HIGHLIGHTED else v
                    else:
                        v = self.MAX_VALUE
                color.setHsv(h, s, v)
                alphaChannelScalar = 1.0
                if type == self.DISABLED:
                    alphaChannelScalar = 0.4
                img.setPixel(x, y,
                             QtGui.qRgba(color.red(), color.green(), color.blue(),
                                   QtGui.qAlpha(pixel)*alphaChannelScalar))
        return QtGui.QPixmap.fromImage(img)

    def sizeHint(self):
        return QtCore.QSize(self.size, self.size)


class MtoACheckableButton(MtoAButton):
    """
    This class represents a checkable render setup button. This supports
    2 different pixmaps, one for each state (checked/unchecked)
    """

    ICON_HIGHLIGHT = QtGui.QColor(113, 142, 184)

    def __init__(self, parent, icon, size=MtoAButton.DEFAULT_BUTTON_SIZE, isEnabled=True, isCheckable=False, isChecked=False):
        super(MtoACheckableButton, self).__init__(parent, icon, size)
        # there is an additional pixmap to draw for the checked state
        self.brighterPixmapChecked = None
        self.enabled = isEnabled
        self.checkable = isCheckable
        self.checked = isChecked
        self.toggled.connect(self.setChecked)

    def setCheckable(self, isCheckable):
        self.checkable = isCheckable
        super(MtoACheckableButton, self).setCheckable(isCheckable)

    def isCheckable(self):
        return self.checkable and self.enabled

    def setChecked(self, checked):
        if self.isCheckable():
            self.checked = checked
            self.repaint()
            super(MtoACheckableButton, self).setChecked(checked)

    def isChecked(self):
        if not self.checkable:
            return False
        return self.checked

    def createOption(self):
        option = super(MtoACheckableButton, self).createOption()

        # The augmented behavior from the base class is that we should keep
        # track of whether the option is checked.
        if not self.isCheckable() or not self.isChecked():
            option.checked = QtWidgets.QStyle.State_Off
        else:
            option.checked = QtWidgets.QStyle.State_On

        return option

    def createPixmap(self, option):
        # We have to choose the proper pixmap depending on the state of the
        # button (checked/unchecked)
        if option.checked == QtWidgets.QStyle.State_On:
            return option.icon.pixmap(self.size, self.size, state=QtGui.QIcon.On)
        else:
            return option.icon.pixmap(self.size, self.size, state=QtGui.QIcon.Off)

    def drawControl(self, element, option, painter, widget=None):

        # make rectangle for the background
        if option.checked == QtWidgets.QStyle.State_On:
            painter.fillRect(option.rect, self.ICON_HIGHLIGHT)

        super(MtoACheckableButton, self).drawControl(element, option, painter, widget)

    def generatePixmapActiveIcon(self, iconMode, pixmap, option):
        # There are 2 brighter pixmaps to draw in this derived class, the
        # additional one is for when the button is checked.
        if option.checked == QtWidgets.QStyle.State_Off:
            if self.brighterPixmap is None:
                self.brighterPixmap = self.generateHighlightedIconPixmap(pixmap)
            return self.brighterPixmap
        else:
            if self.brighterPixmapChecked is None:
                self.brighterPixmapChecked = self.generateHighlightedIconPixmap(pixmap)
            return self.brighterPixmapChecked
