/*
Copyright libCellML Contributors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

const loadLibCellML = require('libcellml.js/libcellml.common')
const { hhSquidAxon1952 } = require('./resources')

let libcellml = null

describe("Analyser Variable tests", () => {
    let m
    let a
    let am

    beforeAll(async () => {
        libcellml = await loadLibCellML()
    });
    beforeEach(() => {
        const p = new libcellml.Parser(true)
        m = p.parseModel(hhSquidAxon1952)
        a = new libcellml.Analyser()

        a.analyseModel(m)
        expect(a.issueCount()).toBe(0)

        am = a.model()

        expect(am.constantCount()).toBe(0)
        expect(am.computedConstantCount()).toBe(0)
        expect(am.algebraicCount()).toBe(18)
    });
    test('Checking Analyser Variable type.', () => {
        const av = am.algebraicVariable(0)
        expect(av.type().value).toBe(libcellml.AnalyserVariable.Type.ALGEBRAIC.value)
        expect(libcellml.AnalyserVariable.typeAsString(av.type())).toBe("algebraic")
    });
    test('Checking Analyser Variable index.', () => {
        const av = am.algebraicVariable(7)
        expect(av.index()).toBe(2)
    });
    test('Checking Analyser Variable initialising variable.', () => {
        const av = am.algebraicVariable(15)
        expect(av.initialisingVariable().name()).toBe("g_K")
    });
    test('Checking Analyser Variable variable.', () => {
        const av = am.algebraicVariable(10)
        expect(av.variable().name()).toBe("alpha_m")
    });
    test('Checking Analyser Equation equationCount.', () => {
        const av = am.algebraicVariable(14)
        expect(av.equationCount()).toBe(1)
    });
    test('Checking Analyser Variable equations.', () => {
        const av = am.algebraicVariable(14)
        expect(av.equations().size()).toBe(1)
    });
    test('Checking Analyser Variable equation.', () => {
        const av = am.algebraicVariable(14)
        expect(av.equation(0).type().value).toBe(libcellml.AnalyserEquation.Type.VARIABLE_BASED_CONSTANT.value)
    });
 })
