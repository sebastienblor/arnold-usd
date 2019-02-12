

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

    def deleteOperator(self, node, path, operator_type):
        """
        delete the operator returned by querying the target node, path
        and operator type
        """

        raise NotImplementedError(
            "{}.deleteOperator".format(str(self.__class__.__name__)))

    def createOperator(self, node, path, operator_type):
        """
        create a new operator and attach it to the give node with the selection
        attribute set to the path if given

        returns: operator or None
        """

        raise NotImplementedError(
            "{}.createOperator".format(str(self.__class__.__name__)))

    def getOperator(self, node, path, operator_type=None, exact_match=False):
        """
        get the operator for the given node and path
        if operator_typoe is set match against this node type

        returns the operator node name or None
        """

        raise NotImplementedError(
            "{}.getOperator".format(str(self.__class__.__name__)))

    def getOverrides(self, node, path, exact_match=False):
        """
        Get a list of ovverrides currently set for the given path
        on the given node
            node        - name of the procedural node to get the overides
            path        - path that we should find the overrides for

        return list of tuples (param, operation, value, index)

            param       - name of the paramter that is being overriden
            operation   - one of "=", "*=", "+=", "-="
            value       - the value of the override
            index       - the index of the overide in the set_parameter operator

            e.g. [("radius", "+=", 4, 0), ("subdiv_type", "=", "catclark", 1]
        """

        raise NotImplementedError(
            "{}.getOverrides".format(str(self.__class__.__name__)))

    def setOverride(self, node, path, param, operation, value, param_type, is_array=False, index=-1):
        """
        Set the override by adding the override to the given set parameter operator.
            node
            path
            param
            operation
            value
            param_type
            is_Array
            index

        """

        raise NotImplementedError(
            "{}.setOverride".format(str(self.__class__.__name__)))

    def deleteOverride(self, node, path, param):
        """
        delete the override form the given path on the given node
        """

        raise NotImplementedError(
            "{}.deleteOverride".format(str(self.__class__.__name__)))
