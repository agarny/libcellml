#
# Tests the Generator class bindings
#
import unittest


class GeneratorTestCase(unittest.TestCase):
    def test_create_destroy(self):
        from libcellml import Generator

        x = Generator()
        del x

    def test_algebraic_eqn_computed_var_on_rhs(self):
        from libcellml import Analyser
        from libcellml import AnalyserModel
        from libcellml import Generator
        from libcellml.generator import Generator_equationCode
        from libcellml import GeneratorProfile
        from libcellml import GeneratorVariableTracker
        from libcellml import Parser
        from test_resources import file_contents

        p = Parser()
        m = p.parseModel(file_contents('generator/algebraic_eqn_computed_var_on_rhs/model.cellml'))

        a = Analyser()
        a.analyseModel(m)

        am = a.analyserModel()

        self.assertEqual(AnalyserModel.Type.ALGEBRAIC, am.type())

        g = Generator()

        self.assertEqual(file_contents("generator/algebraic_eqn_computed_var_on_rhs/model.h"), g.interfaceCode(am))
        self.assertEqual(file_contents("generator/algebraic_eqn_computed_var_on_rhs/model.c"), g.implementationCode(am))

        profile = GeneratorProfile(GeneratorProfile.Profile.PYTHON)

        self.assertEqual("", g.interfaceCode(am, profile))
        self.assertEqual(file_contents("generator/algebraic_eqn_computed_var_on_rhs/model.py"), g.implementationCode(am, profile))

        self.assertEqual("", g.interfaceCode(am, GeneratorProfile.Profile.PYTHON))
        self.assertEqual(file_contents("generator/algebraic_eqn_computed_var_on_rhs/model.py"), g.implementationCode(am, GeneratorProfile.Profile.PYTHON))

        variable_tracker = GeneratorVariableTracker()

        self.assertEqual("", g.interfaceCode(am, profile, variable_tracker))
        self.assertEqual(file_contents("generator/algebraic_eqn_computed_var_on_rhs/model.py"), g.implementationCode(am, profile, variable_tracker))

        self.assertEqual("x = a", Generator.equationCode(am.analyserEquation(0).ast()))
        self.assertEqual("x = a", Generator_equationCode(am.analyserEquation(0).ast()))

    """
    def test_very_big_model_multiple_times(self):
        from libcellml import Analyser
        from libcellml import AnalyserModel
        from libcellml import Generator
        from libcellml import Parser
        from libcellml import Printer
        from test_resources import file_contents

        def issue_fingerprint(logger):
            fingerprint = ''
            issue_count = 0

            for i in range(logger.issueCount()):
                issue = logger.issue(i)

                issue_count += 1

                fingerprint += f"{issue.level()}|"
                fingerprint += f"{issue.referenceRule()}|"
                fingerprint += f"{issue.item().type()}|"
                fingerprint += f"{issue.url()}|"
                fingerprint += f"{issue.description()}\n"

            return f"issues={issue_count}\n{fingerprint}"

        very_big_model = file_contents('very_big_model.cellml')

        parser = Parser(False)
        printer = Printer()
        analyser = Analyser()
        generator = Generator()

        baseline_parser_fingerprint = ''
        baseline_printed_model = ''
        baseline_analyser_fingerprint = ''
        baseline_analyser_model_type = AnalyserModel.Type.UNKNOWN
        baseline_interface_code = ''
        baseline_implementation_code = ''

        for i in range(10):
            model = parser.parseModel(very_big_model)

            self.assertIsNotNone(model)

            parser_fingerprint = issue_fingerprint(parser)
            printed_model = printer.printModel(model)

            analyser.analyseModel(model)

            analyser_fingerprint = issue_fingerprint(analyser)
            analyser_model = analyser.analyserModel()

            self.assertIsNotNone(analyser_model)

            interface_code = generator.interfaceCode(analyser_model)
            implementation_code = generator.implementationCode(analyser_model)

            if i == 0:
                baseline_parser_fingerprint = parser_fingerprint
                baseline_printed_model = printed_model
                baseline_analyser_fingerprint = analyser_fingerprint
                baseline_analyser_model_type = analyser_model.type()
                baseline_interface_code = interface_code
                baseline_implementation_code = implementation_code
            else:
                self.assertEqual(baseline_parser_fingerprint, parser_fingerprint)
                self.assertEqual(baseline_printed_model, printed_model)
                self.assertEqual(baseline_analyser_fingerprint, analyser_fingerprint)
                self.assertEqual(baseline_analyser_model_type, analyser_model.type())
                self.assertEqual(baseline_interface_code, interface_code)
                self.assertEqual(baseline_implementation_code, implementation_code)
    """


if __name__ == '__main__':
    unittest.main()
