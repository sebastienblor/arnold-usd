

class BaseTransverser(object):
    """Base Transverser class used to declare the common operations of transversers"""

    def dir(self, node, path):
        """
        return a list of children from the given path on the given node
        """
        raise NotImplementedError(
            "{}.dir()".format(str(self.__class__.__name__)))

    def properties(self, node, path):
        """
        return a list of properties from the item at a given path
        on a given node
        """
        raise NotImplementedError(
            "{}.properties".format(str(self.__class__.__name__)))

    def dir(self, node, path):
        """
        return a list of objects directly below the given path
        on the given node
        """
        raise NotImplementedError(
            "{}.dir".format(str(self.__class__.__name__)))

    def properties(self, node, path):

        raise NotImplementedError(
            "{}.properties".format(str(self.__class__.__name__)))

    def createOperator(self, node, path):

        raise NotImplementedError(
            "{}.createOperator".format(str(self.__class__.__name__)))

    def getOperator(self, node, path):

        raise NotImplementedError(
            "{}.getOperator".format(str(self.__class__.__name__)))

    def setOverride(self, node, path, param, value):

        raise NotImplementedError(
            "{}.setOverride".format(str(self.__class__.__name__)))

    def deleteOverride(self, node, path, param):

        raise NotImplementedError(
            "{}.deleteOverride".format(str(self.__class__.__name__)))
