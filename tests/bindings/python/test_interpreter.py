#
# Tests the Interpreter class bindings
#
import math
import unittest


class InterpreterTestCase(unittest.TestCase):

    def test_create_destroy(self):
        from libcellml import Interpreter

        x = Interpreter()
        del x

    def assert_array_equal(self, expected_values, values):
        self.assertEqual(len(expected_values), len(values))

        for i in range(len(expected_values)):
            if math.isnan(expected_values[i]):
                self.assertTrue(math.isnan(values[i]))
            else:
                self.assertEqual(expected_values[i], values[i])

    def test_hodgkin_huxley_squid_axon_model_1952(self):
        from libcellml import Analyser
        from libcellml import AnalyserModel
        from libcellml import Interpreter
        from libcellml import Parser
        from test_resources import file_contents

        p = Parser()
        m = p.parseModel(file_contents('generator/hodgkin_huxley_squid_axon_model_1952/model.cellml'))
        a = Analyser()

        a.analyseModel(m)

        am = a.model()

        self.assertEqual(AnalyserModel.Type.ODE, am.type())

        i = Interpreter()

        self.assertIsNone(i.model())

        i.setModel(am)

        self.assertIsNotNone(i.model())

        self.assertEqual(0.0, i.voi())

        nan_x_4 = 4 * [math.nan]
        nan_x_18 = 18 * [math.nan]

        self.assert_array_equal(nan_x_4, i.states())
        self.assert_array_equal(nan_x_4, i.rates())
        self.assert_array_equal(nan_x_18, i.variables())

        i.initialiseVariables()
        i.computeComputedConstants()
        i.computeRates()
        i.computeVariables()

        self.assert_array_equal(nan_x_4, i.states())
        self.assert_array_equal(nan_x_4, i.rates())
        self.assert_array_equal(nan_x_18, i.variables())


if __name__ == '__main__':
    unittest.main()
