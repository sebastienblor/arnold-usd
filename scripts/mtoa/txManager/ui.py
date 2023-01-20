# -*- coding: utf-8 -*-
from __future__ import absolute_import
import logging
import traceback
import copy
import os
from time import sleep

from mtoa.ui.qt.Qt import QtWidgets, QtCore

import mtoa.ui.qt.utils as qtutils

from maya.app.general.mayaMixin import MayaQWidgetDockableMixin
import maya.cmds as cmds

from ..txManager import lib


logger = logging.getLogger(__name__)


def delete_ui(name):
    '''Delete an unique window based on it's objectName'''
    workspace = name + 'WorkspaceControl'
    # Maya 2017
    if hasattr(cmds, 'workspaceControl'):
        while cmds.workspaceControl(workspace, ex=True, q=True):
            cmds.deleteUI(workspace)

    while cmds.window(name, exists=True):
        cmds.deleteUI(name)


# Unused
filters = [
    'box',
    'triangle',
    'gaussian',
    'catrom',
    'blackman-harris',
    'sinc',
    'lanczos3',
    'radial-lanczos3',
    'mitchell',
    'bspline',
    'disk'
]


(NAME,
 STATUS,
 COLORSPACE,
 TXPATH,
 PATH,
 USAGE) = range(6)


class TxManagerWindow(MayaQWidgetDockableMixin, QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        super(TxManagerWindow, self).__init__(parent=parent)

        self.thread = []
        self.textures = {}
        self.fileToCreate = 0
        # Main attributes
        self.setObjectName('txman_win')
        self.setWindowTitle('Tx Manager')
        self.resize(1024, 600)

        # Menu
        self.menu_bar = QtWidgets.QMenuBar(self)
        self.setMenuBar(self.menu_bar)
        self.settings_menu = QtWidgets.QMenu('Settings')
        self.settings_menu.setDisabled(True)

        self.persistent_action = QtWidgets.QAction(
            'Persistent settings',
            self.settings_menu
        )
        self.persistent_action.setCheckable(True)

        self.refresh_on_load = QtWidgets.QAction(
            'Refresh on load',
            self.settings_menu
        )
        self.refresh_on_load.setCheckable(True)

        self.settings_menu.addAction(self.persistent_action)
        self.settings_menu.addAction(self.refresh_on_load)

        self.sorting_menu = QtWidgets.QMenu('Sort by')
        self.sort_status = QtWidgets.QAction('Status', self.sorting_menu)
        self.sort_name = QtWidgets.QAction('Name', self.sorting_menu)
        self.sort_path = QtWidgets.QAction('Path', self.sorting_menu)
        self.sorting_menu.addAction(self.sort_status)
        self.sorting_menu.addAction(self.sort_name)
        self.sorting_menu.addAction(self.sort_path)

        action_grp = QtWidgets.QActionGroup(self)
        self.sort_status.setCheckable(True)
        self.sort_status.setChecked(True)
        self.sort_status.setActionGroup(action_grp)
        self.sort_name.setCheckable(True)
        self.sort_name.setActionGroup(action_grp)
        self.sort_path.setCheckable(True)
        self.sort_path.setActionGroup(action_grp)

        self.menu_bar.addMenu(self.settings_menu)
        self.menu_bar.addMenu(self.sorting_menu)

        # Central widet
        self.central_widget = QtWidgets.QWidget()
        self.central_layout = QtWidgets.QHBoxLayout(self.central_widget)
        self.setCentralWidget(self.central_widget)

        self.splitter = QtWidgets.QSplitter(self.central_widget)
        self.splitter.setOrientation(QtCore.Qt.Horizontal)
        self.splitter.setObjectName("splitter")

        # LEFT
        self.left_grp = QtWidgets.QGroupBox('Texture list', self.splitter)
        self.left_layout = QtWidgets.QVBoxLayout(self.left_grp)

        self.list_action_layout = QtWidgets.QHBoxLayout()
        self.refresh_btn = QtWidgets.QPushButton('Refresh')
        self.select_untiled_btn = QtWidgets.QPushButton('Select Untiled')
        self.select_all_btn = QtWidgets.QPushButton('Select All')
        self.select_missing_btn = QtWidgets.QPushButton('Select Missing')
        self.relink_btn = QtWidgets.QPushButton('Relink ...')

        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)

        self.list_action_layout.addWidget(self.refresh_btn)
        self.list_action_layout.addWidget(self.select_all_btn)
        self.list_action_layout.addWidget(self.select_untiled_btn)
        self.list_action_layout.addWidget(self.select_missing_btn)
        self.list_action_layout.addWidget(self.relink_btn)
        self.list_action_layout.addItem(spacerItem)

        # Texture list
        self.texture_list = QtWidgets.QTreeWidget()
        self.texture_list.setColumnCount(5)
        self.texture_list.setHeaderLabels(['Name', 'Status', 'Colorspace', 'Tx Path', 'Path', 'Nodes'])
        self.texture_list.setSelectionMode(self.texture_list.ExtendedSelection)

        self.info_layout = QtWidgets.QFormLayout()
        self.info_layout.setFieldGrowthPolicy(
            self.info_layout.AllNonFixedFieldsGrow)

        # Info
        # self.colorspace = QtWidgets.QComboBox()  #  Hiding the colrospace widget for now, we don't want artist changing this from the TX manager
        # self.info_hastx = QtWidgets.QLabel()
        # self.info_layout.addRow('Colorspace:', self.colorspace)
        # self.info_layout.addRow('Has .tx:', self.info_hastx)

        # LEFT assingments
        self.left_layout.addLayout(self.list_action_layout)
        self.left_layout.addWidget(self.texture_list)
        self.left_layout.addLayout(self.info_layout)

        # RIGHT
        self.right_frame = QtWidgets.QFrame(self.splitter)
        self.right_frame.setMaximumWidth(qtutils.dpiScale(500))
        self.right_layout = QtWidgets.QVBoxLayout(self.right_frame)

        # Global options
        self.options_group = QtWidgets.QGroupBox('Options')
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Maximum)
        self.options_group.setSizePolicy(sizePolicy)
        self.options_layout = QtWidgets.QHBoxLayout(self.options_group)
        self.options_layout_left = QtWidgets.QFormLayout()
        self.options_layout_right = QtWidgets.QFormLayout()
        self.options_layout.addLayout(self.options_layout_left)
        self.options_layout.addLayout(self.options_layout_right)

        # Radio
        self.radio_scene = QtWidgets.QRadioButton('Scene textures')
        self.radio_scene.setChecked(True)
        self.radio_folder = QtWidgets.QRadioButton('Folder textures')
        self.radio_layout = QtWidgets.QVBoxLayout()
        self.radio_layout.addWidget(self.radio_scene)
        self.radio_layout.addWidget(self.radio_folder)

        self.options_layout_left.addRow('Use', self.radio_layout)

        # TX global options
        # self.local_export = QtWidgets.QCheckBox()
        # self.local_export.setChecked(True)
        # self.options_layout_right.addRow('Create locally', self.local_export)

        # Scene options
        self.scene_options_group = QtWidgets.QGroupBox('Scene Options')
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Maximum)
        self.scene_options_group.setSizePolicy(sizePolicy)
        self.scene_options_layout = QtWidgets.QFormLayout(
            self.scene_options_group)

        self.scene_include_layout = QtWidgets.QVBoxLayout()
        self.scene_include_actions_layout = QtWidgets.QHBoxLayout()

        self.scene_include = QtWidgets.QListWidget()
        self.scene_include.addItems(lib.scene_default_texture_scan)
        self.scene_include_add = QtWidgets.QPushButton('Add')
        self.scene_include_rm = QtWidgets.QPushButton('Remove')
        self.scene_include_actions_layout.addWidget(self.scene_include_add)
        self.scene_include_actions_layout.addWidget(self.scene_include_rm)
        self.scene_include_layout.addWidget(self.scene_include)
        self.scene_include_layout.addLayout(self.scene_include_actions_layout)

        self.scene_options_layout.addRow(
            'Scan attributes', self.scene_include_layout)

        # Folder Options
        self.folder_options_group = QtWidgets.QGroupBox('Folder Options')
        self.folder_options_group.setHidden(True)
        self.folder_options_layout = QtWidgets.QFormLayout(
            self.folder_options_group)

        self.folder_filepath = QtWidgets.QLineEdit()
        self.folder_browse = QtWidgets.QPushButton('Browse')
        self.folder_recursive = QtWidgets.QCheckBox()
        self.folder_options_layout.addRow('Subfolders', self.folder_recursive)
        self.folder_options_layout.addRow('Folder', self.folder_filepath)
        self.folder_options_layout.addRow('', self.folder_browse)

        # Tx  Options
        self.tx_options_group = QtWidgets.QGroupBox('Tx Options')
        self.tx_options_layout = QtWidgets.QVBoxLayout(
            self.tx_options_group)
        self.tx_manual_options_widget = QtWidgets.QWidget()
        self.tx_manual_options_layout = QtWidgets.QVBoxLayout(
            self.tx_manual_options_widget)
        self.tx_manual_options_layout_top = QtWidgets.QHBoxLayout()
        self.tx_manual_options_layout_left = QtWidgets.QFormLayout()
        self.tx_manual_options_layout_right = QtWidgets.QFormLayout()
        self.tx_manual_options_layout_bottom = QtWidgets.QFormLayout()
        self.tx_manual_options_layout_top.addLayout(self.tx_manual_options_layout_left)
        self.tx_manual_options_layout_top.addLayout(self.tx_manual_options_layout_right)
        self.tx_manual_options_layout.addLayout(self.tx_manual_options_layout_top)
        self.tx_manual_options_layout.addLayout(self.tx_manual_options_layout_bottom)

        self.tx_update = QtWidgets.QCheckBox()
        self.tx_verbose = QtWidgets.QCheckBox()
        self.tx_checknan = QtWidgets.QCheckBox()
        self.tx_stats = QtWidgets.QCheckBox()
        self.tx_unpremult = QtWidgets.QCheckBox()
        self.tx_preset = QtWidgets.QComboBox()
        self.tx_threads = QtWidgets.QSpinBox()
        self.tx_preset.addItems(['oiio', 'prman', 'No preset'])
        self.tx_extra_args = QtWidgets.QLineEdit()
        self.tx_preview = QtWidgets.QLineEdit()

        self.tx_use_autotx = QtWidgets.QCheckBox('Default Options (auto tx)')

        # Defaults
        self.tx_update.setChecked(True)
        self.tx_verbose.setChecked(True)
        self.tx_unpremult.setChecked(True)
        self.tx_stats.setChecked(True)
        self.tx_checknan.setChecked(True)
        self.tx_preview.setReadOnly(True)

        self.tx_manual_options_widget.setDisabled(True)
        self.tx_use_autotx.setChecked(True)

        self.tx_manual_options_layout_left.addRow('Update', self.tx_update)
        self.tx_manual_options_layout_left.addRow('Verbose', self.tx_verbose)
        self.tx_manual_options_layout_left.addRow('Check NaNs', self.tx_checknan)
        self.tx_manual_options_layout_left.addRow('Threads', self.tx_threads)
        self.tx_manual_options_layout_right.addRow('Print stats', self.tx_stats)
        self.tx_manual_options_layout_right.addRow('Unpremult', self.tx_unpremult)
        self.tx_manual_options_layout_right.addRow('Preset', self.tx_preset)
        self.tx_manual_options_layout_bottom.addRow('Extra args', self.tx_extra_args)
        self.tx_manual_options_layout_bottom.addRow('Preview', self.tx_preview)

        self.tx_options_layout.addWidget(self.tx_manual_options_widget)
        self.tx_options_layout.addWidget(self.tx_use_autotx)

        # Actions
        self.actions_group = QtWidgets.QGroupBox('Actions')
        self.actions_layout = QtWidgets.QHBoxLayout(self.actions_group)

        self.action_create = QtWidgets.QPushButton('Create .tx for selected')
        self.action_rm = QtWidgets.QPushButton('Remove .tx for selected')

        self.actions_layout.addWidget(self.action_create)
        self.actions_layout.addWidget(self.action_rm)

        self.right_layout.addWidget(self.options_group)
        self.right_layout.addWidget(self.scene_options_group)
        self.right_layout.addWidget(self.folder_options_group)
        self.right_layout.addWidget(self.tx_options_group)

        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.right_layout.addItem(spacerItem)

        self.right_layout.addWidget(self.actions_group)

        # CENTRAL assignments
        self.central_layout.addWidget(self.splitter)

        # Signals
        self.texture_list.itemSelectionChanged.connect(
            self.on_selection_changed)

        self.refresh_btn.clicked.connect(self.on_refresh)
        self.select_all_btn.clicked.connect(self.on_select_all)
        self.select_untiled_btn.clicked.connect(self.on_select_untiled)
        self.select_missing_btn.clicked.connect(self.on_select_missing)
        self.relink_btn.clicked.connect(self.on_relink_textures)

        self.radio_scene.toggled.connect(self.on_scene_source_changed)
        # self.scene_include.itemSelectionChanged.connect(self.set_editable)
        self.scene_include_add.clicked.connect(self.on_include_add)
        self.scene_include_rm.clicked.connect(self.on_include_rm)

        self.folder_browse.clicked.connect(self.on_browse_folder)
        self.sort_name.triggered.connect(self.update_texture_display)
        self.sort_path.triggered.connect(self.update_texture_display)
        self.sort_status.triggered.connect(self.update_texture_display)

        self.tx_update.toggled.connect(self.on_update_args)
        self.tx_verbose.toggled.connect(self.on_update_args)
        self.tx_checknan.toggled.connect(self.on_update_args)
        self.tx_stats.toggled.connect(self.on_update_args)
        self.tx_threads.valueChanged.connect(self.on_update_args)
        self.tx_unpremult.toggled.connect(self.on_update_args)
        self.tx_preset.currentIndexChanged.connect(self.on_update_args)
        self.tx_extra_args.textChanged.connect(self.on_update_args)

        self.tx_use_autotx.toggled.connect(self.tx_manual_options_widget.setDisabled)

        # self.colorspace.currentIndexChanged.connect(self.on_colorspace_change)

        self.action_create.clicked.connect(self.on_create_tx)
        self.action_rm.clicked.connect(self.on_delete_tx)

        # Initialization
        self.on_update_args()

    def set_editable(self):
        '''Helper to set scan attributes editable'''
        for item in self.scene_include.selectedItems():
            item.setFlags(item.flags() | QtCore.Qt.ItemIsEditable)

    def on_include_add(self):
        '''Callback for scan attribute add'''
        self.scene_include.addItem('<node_type>.<attribute>')

    def on_include_rm(self):
        '''Callback for scan attribute removal'''
        self.scene_include.takeItem(self.scene_include.currentRow())

    def on_scene_source_changed(self, state):
        '''Callback for chaanges in texture scan method'''
        self.scene_options_group.setHidden(not state)
        self.folder_options_group.setHidden(state)

    def on_browse_folder(self):
        '''Callback for folder browsing'''
        path = cmds.fileDialog2(fm=3, cap='Texture folder')
        if not path:
            return
        path = path[0]

        self.folder_filepath.setText(path)
        self.on_refresh()

    def on_refresh(self):
        '''Scans for textures based on the current options'''
        if self.radio_scene.isChecked():
            includes = set()

            for i in range(self.scene_include.count()):
                includes.add(self.scene_include.item(i).text())
            textures = lib.get_scanned_files(includes)

        else:
            textures = lib.get_folder_textures(
                self.folder_filepath.text(),
                subfolders=self.folder_recursive.isChecked()
            )

        self.textures = textures
        self.update_texture_display()

    def update_texture_display(self):
        '''Refreshes the texture list with the texture cache and current
        visualization options'''

        self.texture_list.clear()
        sort_key = 'status'
        if self.sort_name.isChecked():
            sort_key = 'name'
        elif self.sort_path.isChecked():
            sort_key = 'path'

        sorted_textures = sorted(
            self.textures.items(), key=lambda x: x[1][sort_key])

        for texture, data in sorted_textures:
            data = copy.deepcopy(data)

            item = QtWidgets.QTreeWidgetItem(self.texture_list)
            item.setData(0, QtCore.Qt.UserRole, data)
            item.setText(NAME, data['name'])
            item.setText(STATUS, data['status'])
            item.setText(COLORSPACE, data['colorspace'])
            item.setText(TXPATH, data['txpath'])
            item.setText(PATH, data['path'])
            item.setText(USAGE, ','.join([x.split('.')[0] for x in data['usage']]))
            self.texture_list.addTopLevelItem(item)
            # loop the texture nodes and populate the child information
            for node in data['usage']:
                child_data = copy.deepcopy(data['usage'][node])
                child_data['name'] = node
                child_item = QtWidgets.QTreeWidgetItem(item)
                child_item.setData(0, QtCore.Qt.UserRole, child_data)
                child_item.setText(NAME, child_data['name'])
                child_item.setText(STATUS, child_data['status'])
                child_item.setText(COLORSPACE, child_data['colorspace'])
                child_item.setText(TXPATH, child_data['txpath'])
                child_item.setText(PATH, child_data['path'])
                item.addChild(child_item)

    def on_update_args(self):
        '''Callback for any changes in maketx argument widgets'''
        args = self.get_tx_args()
        args.insert(0, 'maketx')
        args.append('%s')
        self.tx_preview.setText(' '.join(args))

    def get_tx_args(self):
        '''Helper function to build the maketx command arguments'''
        preset = self.tx_preset.currentText()
        preset = None if preset == 'No preset' else preset
        args = lib.build_tx_arguments(
            update=self.tx_update.isChecked(),
            verbose=self.tx_verbose.isChecked(),
            nans=self.tx_checknan.isChecked(),
            stats=self.tx_stats.isChecked(),
            unpremutl=self.tx_unpremult.isChecked(),
            threads=self.tx_threads.value(),
            preset=preset,
            extra_args=self.tx_extra_args.text()
        )
        return args

    def get_selected_textures(self):
        '''returns list of texture data dictionaries for the selected items'''
        selectedTextures = []
        texture_selection = self.texture_list.selectedItems()

        for item in texture_selection:
            data = item.data(0, QtCore.Qt.UserRole)
            data['index'] = self.texture_list.indexOfTopLevelItem(item)
            selectedTextures.append(data)

        return selectedTextures

    def on_selection_changed(self):
        '''Callback for selection change in texture list'''
        cmds.select(cl=True)
        texture_selection = self.texture_list.selectedItems()

        # update colorspace combo
        # colorspaces = set()
        for item in texture_selection:
            data = item.data(0, QtCore.Qt.UserRole)
            name = data['name']
            if 'usage' in data:
                usage_nodes = list(data['usage'].keys())
                selection = usage_nodes
            elif cmds.objExists(name):
                selection=name
            
            cmds.select(selection, add=True)

        #     colorspaces.add(data['colorspace'])

        # self._dontupdate = True
        # self.colorspace.clear()
        # if not len(colorspaces):
        #     return

        # if len(colorspaces) > 1:
        #     self.colorspace.addItem('Multiple')

        # default = cmds.colorManagementPrefs(q=True, inputSpaceNames=True)
        # self.colorspace.addItems(default)
        # if len(colorspaces) == 1:
        #     colorspace = list(colorspaces)[0]
        #     index = self.colorspace.findText(colorspace)
        #     self.colorspace.setCurrentIndex(index)

        # TODO update the tx info

    def on_colorspace_change(self):
        '''Callback for changes in colorspace QComboBox'''
        if self._dontupdate:
            self._dontupdate = False
            return

        for item in self.texture_list.selectedItems():
            cs = self.colorspace.currentText()
            if cs.lower() == 'multiple':
                return

            data = item.data(0, QtCore.Qt.UserRole)
            data['colorspace'] = cs
            item.setData(0, QtCore.Qt.UserRole, data)
            item.setText(COLORSPACE, cs)

    def on_select_untiled(self):
        '''Callback for untiled texture selection'''
        self.texture_list.selectionModel().clear()

        for i in range(self.texture_list.model().rowCount()):
            index = self.texture_list.model().index(i,0)
            item = self.texture_list.itemFromIndex(index)
            data = item.data(0, QtCore.Qt.UserRole)

            if data['status'] == 'notx':
                self.texture_list.setItemSelected(
                    item,
                    True
                )

    def on_select_missing(self):
        '''Callback for untiled texture selection'''
        self.texture_list.selectionModel().clear()

        for i in range(self.texture_list.model().rowCount()):
            index = self.texture_list.model().index(i,0)
            item = self.texture_list.itemFromIndex(index)
            data = item.data(0, QtCore.Qt.UserRole)

            if data['status'] == 'missing':
                self.texture_list.setItemSelected(
                    item,
                    True
                )

    def on_relink_textures(self):
        '''Callback for relinking texture selection to a new folder'''

        folder = cmds.fileDialog2(fm=3, cap='Texture folder')
        if not folder:
            return
        folder = folder[0]

        texture_selection = self.texture_list.selectedItems()

        for item in texture_selection:
            data = item.data(0, QtCore.Qt.UserRole)

            dn, bn = os.path.split(data['path'])
            newpath = os.path.join(folder, bn)
            # get the file nodes
            for fnattr in data['usage']:
                cmds.setAttr(fnattr, newpath, type="string")

        self.on_refresh()

    def on_select_all(self):
        # self.texture_list.selectionModel().clear()
        model = self.texture_list.model()
        first = model.index(0, 0)
        last = model.index(model.rowCount()-1, 4)
        selection = QtCore.QItemSelection(first, last)
        self.texture_list.selectionModel().select(selection, QtCore.QItemSelectionModel.Select)

    def on_create_tx(self):
        '''Callback for TX creation'''

        self.action_create.setEnabled(False)
        self.action_rm.setEnabled(False)
        if not self.thread:
            self.thread = lib.MakeTxThread(self, self)
            self.thread.finished.connect(self.on_finish_tx)

        progress = QtWidgets.QProgressDialog("processing textures...", "Abort", 0, 2, self)
        progress.forceShow()
        progress.setWindowModality(QtCore.Qt.WindowModal)

        progress.canceled.connect(self.on_cancel_tx)
        self.thread.maxProgress.connect(progress.setMaximum)
        self.thread.progress.connect(progress.setValue)
        self.thread.finished.connect(progress.deleteLater)

        self.thread.start()

    def on_delete_tx(self):
        '''Callback for TX deletion'''
        for item in self.texture_list.selectedItems():
            data = item.data(0, QtCore.Qt.UserRole)

            if data['status'] != 'hastx':
                logger.warning('Skipping file "%s".' % data['path'])
                continue

            logger.info('Deleting "%s"...' % data['txpath'])
            try:
                tx_files = lib.makeTx.expandFilenameWithSearchPaths(data['txpath'])
                for tx in tx_files:
                    os.remove(tx)
            except Exception:
                traceback.print_exc()
                logger.error('Failed to remove tx file "%s"' % data['path'])

        self.on_refresh()

    def on_cancel_tx(self):
        # stop the thread
        if self.thread and self.thread.isRunning():
            self.thread.cancel_tx()
            while self.thread.isRunning():
                continue

        self.on_refresh()

    def on_finish_tx(self):
        # stop the thread
        self.action_create.setEnabled(True)
        self.action_rm.setEnabled(True)
        self.on_refresh()

    def set_status(self, row, status="N/A"):
        item = self.texture_list.topLevelItem(row)
        if item:
            data = item.data(0, QtCore.Qt.UserRole)
            data['status'] = status
            item.setData(0, QtCore.Qt.UserRole, data)
            item.setText(1, status)

    def get_status(self, row):
        item = self.texture_list.topLevelItem(row)
        if item:
            data = item.data(0, QtCore.Qt.UserRole)
            return data['status']

        return None

    def get_use_autotx(self):
        return self.tx_use_autotx.isChecked()

    def update_data(self, row):
        item = self.texture_list.topLevelItem(row)
        if item:
            data = item.data(0, QtCore.Qt.UserRole)
            new_data = lib.update_texture_data(data)
            item.setData(0, QtCore.Qt.UserRole, new_data)
            item.setText(NAME, new_data['name'])
            item.setText(STATUS, new_data['status'])
            item.setText(COLORSPACE, new_data['colorspace'])
            item.setText(TXPATH, new_data['txpath'])
            item.setText(PATH, new_data['path'])
            item.setText(USAGE, ','.join([x.split('.')[0] for x in new_data['usage']]))


def show():
    global txman_win
    delete_ui('txman_win')

    txman_win = TxManagerWindow()
    txman_win.show(dockable=True)
    txman_win.on_refresh()


if __name__ == '__main__':
    show()
