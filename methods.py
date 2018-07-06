import os
from compat import iteritems, itervalues, open_utf8, escape_string


def add_source_files(self, sources, filetype, lib_env=None, shared=False):
    import glob
    import string
    # if not lib_objects:
    if not lib_env:
        lib_env = self
    if type(filetype) == type(""):

        dir = self.Dir('.').abspath
        list = glob.glob(dir + "/" + filetype)
        for f in list:
            sources.append(self.Object(f))
    else:
        for f in filetype:
            sources.append(self.Object(f))



class LegacyGLHeaderStruct:

    def __init__(self):
        self.vertex_lines = []
        self.fragment_lines = []
        self.uniforms = []
        self.attributes = []
        self.feedbacks = []
        self.fbos = []
        self.conditionals = []
        self.enums = {}
        self.texunits = []
        self.texunit_names = []
        self.ubos = []
        self.ubo_names = []

        self.vertex_included_files = []
        self.fragment_included_files = []

        self.reading = ""
        self.line_offset = 0
        self.vertex_offset = 0
        self.fragment_offset = 0


def include_file_in_legacygl_header(filename, header_data, depth):
    fs = open(filename, "r")
    line = fs.readline()

    while(line):

        if (line.find("[vertex]") != -1):
            header_data.reading = "vertex"
            line = fs.readline()
            header_data.line_offset += 1
            header_data.vertex_offset = header_data.line_offset
            continue

        if (line.find("[fragment]") != -1):
            header_data.reading = "fragment"
            line = fs.readline()
            header_data.line_offset += 1
            header_data.fragment_offset = header_data.line_offset
            continue

        while(line.find("#include ") != -1):
            includeline = line.replace("#include ", "").strip()[1:-1]

            import os.path

            included_file = os.path.relpath(os.path.dirname(filename) + "/" + includeline)
            if (not included_file in header_data.vertex_included_files and header_data.reading == "vertex"):
                header_data.vertex_included_files += [included_file]
                if(include_file_in_legacygl_header(included_file, header_data, depth + 1) == None):
                    print("Error in file '" + filename + "': #include " + includeline + "could not be found!")
            elif (not included_file in header_data.fragment_included_files and header_data.reading == "fragment"):
                header_data.fragment_included_files += [included_file]
                if(include_file_in_legacygl_header(included_file, header_data, depth + 1) == None):
                    print("Error in file '" + filename + "': #include " + includeline + "could not be found!")

            line = fs.readline()

        if (line.find("#ifdef ") != -1 or line.find("#elif defined(") != -1):
            if (line.find("#ifdef ") != -1):
                ifdefline = line.replace("#ifdef ", "").strip()
            else:
                ifdefline = line.replace("#elif defined(", "").strip()
                ifdefline = ifdefline.replace(")", "").strip()

            if (line.find("_EN_") != -1):
                enumbase = ifdefline[:ifdefline.find("_EN_")]
                ifdefline = ifdefline.replace("_EN_", "_")
                line = line.replace("_EN_", "_")
                if (enumbase not in header_data.enums):
                    header_data.enums[enumbase] = []
                if (ifdefline not in header_data.enums[enumbase]):
                    header_data.enums[enumbase].append(ifdefline)

            elif (not ifdefline in header_data.conditionals):
                header_data.conditionals += [ifdefline]

        if (line.find("uniform") != -1 and line.lower().find("texunit:") != -1):
            # texture unit
            texunitstr = line[line.find(":") + 1:].strip()
            if (texunitstr == "auto"):
                texunit = "-1"
            else:
                texunit = str(int(texunitstr))
            uline = line[:line.lower().find("//")]
            uline = uline.replace("uniform", "")
            uline = uline.replace("highp", "")
            uline = uline.replace(";", "")
            lines = uline.split(",")
            for x in lines:

                x = x.strip()
                x = x[x.rfind(" ") + 1:]
                if (x.find("[") != -1):
                    # unfiorm array
                    x = x[:x.find("[")]

                if (not x in header_data.texunit_names):
                    header_data.texunits += [(x, texunit)]
                    header_data.texunit_names += [x]

        elif (line.find("uniform") != -1 and line.lower().find("ubo:") != -1):
            # uniform buffer object
            ubostr = line[line.find(":") + 1:].strip()
            ubo = str(int(ubostr))
            uline = line[:line.lower().find("//")]
            uline = uline[uline.find("uniform") + len("uniform"):]
            uline = uline.replace("highp", "")
            uline = uline.replace(";", "")
            uline = uline.replace("{", "").strip()
            lines = uline.split(",")
            for x in lines:

                x = x.strip()
                x = x[x.rfind(" ") + 1:]
                if (x.find("[") != -1):
                    # unfiorm array
                    x = x[:x.find("[")]

                if (not x in header_data.ubo_names):
                    header_data.ubos += [(x, ubo)]
                    header_data.ubo_names += [x]

        elif (line.find("uniform") != -1 and line.find("{") == -1 and line.find(";") != -1):
            uline = line.replace("uniform", "")
            uline = uline.replace(";", "")
            lines = uline.split(",")
            for x in lines:

                x = x.strip()
                x = x[x.rfind(" ") + 1:]
                if (x.find("[") != -1):
                    # unfiorm array
                    x = x[:x.find("[")]

                if (not x in header_data.uniforms):
                    header_data.uniforms += [x]

        if (line.strip().find("attribute ") == 0 and line.find("attrib:") != -1):
            uline = line.replace("in ", "")
            uline = uline.replace("attribute ", "")
            uline = uline.replace("highp ", "")
            uline = uline.replace(";", "")
            uline = uline[uline.find(" "):].strip()

            if (uline.find("//") != -1):
                name, bind = uline.split("//")
                if (bind.find("attrib:") != -1):
                    name = name.strip()
                    bind = bind.replace("attrib:", "").strip()
                    header_data.attributes += [(name, bind)]

        if (line.strip().find("out ") == 0 and line.find("tfb:") != -1):
            uline = line.replace("out ", "")
            uline = uline.replace("highp ", "")
            uline = uline.replace(";", "")
            uline = uline[uline.find(" "):].strip()

            if (uline.find("//") != -1):
                name, bind = uline.split("//")
                if (bind.find("tfb:") != -1):
                    name = name.strip()
                    bind = bind.replace("tfb:", "").strip()
                    header_data.feedbacks += [(name, bind)]

        line = line.replace("\r", "")
        line = line.replace("\n", "")

        if (header_data.reading == "vertex"):
            header_data.vertex_lines += [line]
        if (header_data.reading == "fragment"):
            header_data.fragment_lines += [line]

        line = fs.readline()
        header_data.line_offset += 1

    fs.close()

    return header_data


def build_legacygl_header(filename, include, class_suffix, output_attribs, gles2=False):

    header_data = LegacyGLHeaderStruct()
    include_file_in_legacygl_header(filename, header_data, 0)

    out_file = filename + ".gen.h"
    fd = open(out_file, "w")

    enum_constants = []

    fd.write("/* WARNING, THIS FILE WAS GENERATED, DO NOT EDIT */\n")

    out_file_base = out_file
    out_file_base = out_file_base[out_file_base.rfind("/") + 1:]
    out_file_base = out_file_base[out_file_base.rfind("\\") + 1:]
    out_file_ifdef = out_file_base.replace(".", "_").upper()
    fd.write("#ifndef " + out_file_ifdef + class_suffix + "_120\n")
    fd.write("#define " + out_file_ifdef + class_suffix + "_120\n")

    out_file_class = out_file_base.replace(".glsl.gen.h", "").title().replace("_", "").replace(".", "") + "Shader" + class_suffix
    fd.write("\n\n")
    fd.write("#include \"" + include + "\"\n\n\n")
    fd.write("class " + out_file_class + " : public Shader" + class_suffix + " {\n\n")
    fd.write("\t virtual String get_shader_name() const { return \"" + out_file_class + "\"; }\n")

    fd.write("public:\n\n")

    if (len(header_data.conditionals)):
        fd.write("\tenum Conditionals {\n")
        for x in header_data.conditionals:
            fd.write("\t\t" + x.upper() + ",\n")
        fd.write("\t};\n\n")

    if (len(header_data.uniforms)):
        fd.write("\tenum Uniforms {\n")
        for x in header_data.uniforms:
            fd.write("\t\t" + x.upper() + ",\n")
        fd.write("\t};\n\n")

    fd.write("\t_FORCE_INLINE_ int get_uniform(Uniforms p_uniform) const { return _get_uniform(p_uniform); }\n\n")
    if (len(header_data.conditionals)):

        fd.write("\t_FORCE_INLINE_ void set_conditional(Conditionals p_conditional,bool p_enable)  {  _set_conditional(p_conditional,p_enable); }\n\n")
    fd.write("\t#define _FU if (get_uniform(p_uniform)<0) return; ERR_FAIL_COND( get_active()!=this );\n\n ")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, float p_value) { _FU glUniform1f(get_uniform(p_uniform),p_value); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, double p_value) { _FU glUniform1f(get_uniform(p_uniform),p_value); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, uint8_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, int8_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, uint16_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, int16_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, uint32_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, int32_t p_value) { _FU glUniform1i(get_uniform(p_uniform),p_value); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const Color& p_color) { _FU GLfloat col[4]={p_color.r,p_color.g,p_color.b,p_color.a}; glUniform4fv(get_uniform(p_uniform),1,col); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const Vector2& p_vec2) { _FU GLfloat vec2[2]={p_vec2.x,p_vec2.y}; glUniform2fv(get_uniform(p_uniform),1,vec2); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const Vector3& p_vec3) { _FU GLfloat vec3[3]={p_vec3.x,p_vec3.y,p_vec3.z}; glUniform3fv(get_uniform(p_uniform),1,vec3); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, float p_a, float p_b) { _FU glUniform2f(get_uniform(p_uniform),p_a,p_b); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, float p_a, float p_b, float p_c) { _FU glUniform3f(get_uniform(p_uniform),p_a,p_b,p_c); }\n\n")
    fd.write("\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, float p_a, float p_b, float p_c, float p_d) { _FU glUniform4f(get_uniform(p_uniform),p_a,p_b,p_c,p_d); }\n\n")

    fd.write("""\t_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const Transform& p_transform) {  _FU

		const Transform &tr = p_transform;

		GLfloat matrix[16]={ /* build a 16x16 matrix */
			tr.basis.elements[0][0],
			tr.basis.elements[1][0],
			tr.basis.elements[2][0],
			0,
			tr.basis.elements[0][1],
			tr.basis.elements[1][1],
			tr.basis.elements[2][1],
			0,
			tr.basis.elements[0][2],
			tr.basis.elements[1][2],
			tr.basis.elements[2][2],
			0,
			tr.origin.x,
			tr.origin.y,
			tr.origin.z,
			1
		};


                glUniformMatrix4fv(get_uniform(p_uniform),1,false,matrix);


	}

	""")

    fd.write("""_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const Transform2D& p_transform) {  _FU

		const Transform2D &tr = p_transform;

		GLfloat matrix[16]={ /* build a 16x16 matrix */
			tr.elements[0][0],
			tr.elements[0][1],
			0,
			0,
			tr.elements[1][0],
			tr.elements[1][1],
			0,
			0,
			0,
			0,
			1,
			0,
			tr.elements[2][0],
			tr.elements[2][1],
			0,
			1
		};


                glUniformMatrix4fv(get_uniform(p_uniform),1,false,matrix);


	}

	""")

    fd.write("""_FORCE_INLINE_ void set_uniform(Uniforms p_uniform, const CameraMatrix& p_matrix) {  _FU

		GLfloat matrix[16];

		for (int i=0;i<4;i++) {
			for (int j=0;j<4;j++) {

				matrix[i*4+j]=p_matrix.matrix[i][j];
			}
		}

		glUniformMatrix4fv(get_uniform(p_uniform),1,false,matrix);
        } """)

    fd.write("\n\n#undef _FU\n\n\n")

    fd.write("\tvirtual void init() {\n\n")

    enum_value_count = 0

    if (len(header_data.enums)):

        fd.write("\t\t//Written using math, given nonstandarity of 64 bits integer constants..\n")
        fd.write("\t\tstatic const Enum _enums[]={\n")

        bitofs = len(header_data.conditionals)
        enum_vals = []

        for xv in header_data.enums:
            x = header_data.enums[xv]
            bits = 1
            amt = len(x)
            while(2**bits < amt):
                bits += 1
            strs = "{"
            for i in range(amt):
                strs += "\"#define " + x[i] + "\\n\","

                v = {}
                v["set_mask"] = "uint64_t(" + str(i) + ")<<" + str(bitofs)
                v["clear_mask"] = "((uint64_t(1)<<40)-1) ^ (((uint64_t(1)<<" + str(bits) + ") - 1)<<" + str(bitofs) + ")"
                enum_vals.append(v)
                enum_constants.append(x[i])

            strs += "NULL}"

            fd.write("\t\t\t{(uint64_t(1<<" + str(bits) + ")-1)<<" + str(bitofs) + "," + str(bitofs) + "," + strs + "},\n")
            bitofs += bits

        fd.write("\t\t};\n\n")

        fd.write("\t\tstatic const EnumValue _enum_values[]={\n")

        enum_value_count = len(enum_vals)
        for x in enum_vals:
            fd.write("\t\t\t{" + x["set_mask"] + "," + x["clear_mask"] + "},\n")

        fd.write("\t\t};\n\n")

    conditionals_found = []
    if (len(header_data.conditionals)):

        fd.write("\t\tstatic const char* _conditional_strings[]={\n")
        if (len(header_data.conditionals)):
            for x in header_data.conditionals:
                fd.write("\t\t\t\"#define " + x + "\\n\",\n")
                conditionals_found.append(x)
        fd.write("\t\t};\n\n")
    else:
        fd.write("\t\tstatic const char **_conditional_strings=NULL;\n")

    if (len(header_data.uniforms)):

        fd.write("\t\tstatic const char* _uniform_strings[]={\n")
        if (len(header_data.uniforms)):
            for x in header_data.uniforms:
                fd.write("\t\t\t\"" + x + "\",\n")
        fd.write("\t\t};\n\n")
    else:
        fd.write("\t\tstatic const char **_uniform_strings=NULL;\n")

    if output_attribs:
        if (len(header_data.attributes)):

            fd.write("\t\tstatic AttributePair _attribute_pairs[]={\n")
            for x in header_data.attributes:
                fd.write("\t\t\t{\"" + x[0] + "\"," + x[1] + "},\n")
            fd.write("\t\t};\n\n")
        else:
            fd.write("\t\tstatic AttributePair *_attribute_pairs=NULL;\n")

    feedback_count = 0

    if (not gles2 and len(header_data.feedbacks)):

        fd.write("\t\tstatic const Feedback _feedbacks[]={\n")
        for x in header_data.feedbacks:
            name = x[0]
            cond = x[1]
            if (cond in conditionals_found):
                fd.write("\t\t\t{\"" + name + "\"," + str(conditionals_found.index(cond)) + "},\n")
            else:
                fd.write("\t\t\t{\"" + name + "\",-1},\n")

            feedback_count += 1

        fd.write("\t\t};\n\n")
    else:
        if gles2:
            pass
        else:
            fd.write("\t\tstatic const Feedback* _feedbacks=NULL;\n")

    if (len(header_data.texunits)):
        fd.write("\t\tstatic TexUnitPair _texunit_pairs[]={\n")
        for x in header_data.texunits:
            fd.write("\t\t\t{\"" + x[0] + "\"," + x[1] + "},\n")
        fd.write("\t\t};\n\n")
    else:
        fd.write("\t\tstatic TexUnitPair *_texunit_pairs=NULL;\n")

    if (not gles2 and len(header_data.ubos)):
        fd.write("\t\tstatic UBOPair _ubo_pairs[]={\n")
        for x in header_data.ubos:
            fd.write("\t\t\t{\"" + x[0] + "\"," + x[1] + "},\n")
        fd.write("\t\t};\n\n")
    else:
        if gles2:
            pass
        else:
            fd.write("\t\tstatic UBOPair *_ubo_pairs=NULL;\n")

    fd.write("\t\tstatic const char _vertex_code[]={\n")
    for x in header_data.vertex_lines:
        for i in range(len(x)):
            fd.write(str(ord(x[i])) + ",")

        fd.write(str(ord('\n')) + ",")
    fd.write("\t\t0};\n\n")

    fd.write("\t\tstatic const int _vertex_code_start=" + str(header_data.vertex_offset) + ";\n")

    fd.write("\t\tstatic const char _fragment_code[]={\n")
    for x in header_data.fragment_lines:
        for i in range(len(x)):
            fd.write(str(ord(x[i])) + ",")

        fd.write(str(ord('\n')) + ",")
    fd.write("\t\t0};\n\n")

    fd.write("\t\tstatic const int _fragment_code_start=" + str(header_data.fragment_offset) + ";\n")

    if output_attribs:
        if gles2:
            fd.write("\t\tsetup(_conditional_strings," + str(len(header_data.conditionals)) + ",_uniform_strings," + str(len(header_data.uniforms)) + ",_attribute_pairs," + str(len(header_data.attributes)) + ", _texunit_pairs," + str(len(header_data.texunits)) + ",_vertex_code,_fragment_code,_vertex_code_start,_fragment_code_start);\n")
        else:
            fd.write("\t\tsetup(_conditional_strings," + str(len(header_data.conditionals)) + ",_uniform_strings," + str(len(header_data.uniforms)) + ",_attribute_pairs," + str(len(header_data.attributes)) + ", _texunit_pairs," + str(len(header_data.texunits)) + ",_ubo_pairs," + str(len(header_data.ubos)) + ",_feedbacks," + str(feedback_count) + ",_vertex_code,_fragment_code,_vertex_code_start,_fragment_code_start);\n")
    else:
        if gles2:
            fd.write("\t\tsetup(_conditional_strings," + str(len(header_data.conditionals)) + ",_uniform_strings," + str(len(header_data.uniforms)) + ",_texunit_pairs," + str(len(header_data.texunits)) + ",_enums," + str(len(header_data.enums)) + ",_enum_values," + str(enum_value_count) + ",_vertex_code,_fragment_code,_vertex_code_start,_fragment_code_start);\n")
        else:
            fd.write("\t\tsetup(_conditional_strings," + str(len(header_data.conditionals)) + ",_uniform_strings," + str(len(header_data.uniforms)) + ",_texunit_pairs," + str(len(header_data.texunits)) + ",_enums," + str(len(header_data.enums)) + ",_enum_values," + str(enum_value_count) + ",_ubo_pairs," + str(len(header_data.ubos)) + ",_feedbacks," + str(feedback_count) + ",_vertex_code,_fragment_code,_vertex_code_start,_fragment_code_start);\n")

    fd.write("\t}\n\n")

    if (len(enum_constants)):

        fd.write("\tenum EnumConditionals {\n")
        for x in enum_constants:
            fd.write("\t\t" + x.upper() + ",\n")
        fd.write("\t};\n\n")
        fd.write("\tvoid set_enum_conditional(EnumConditionals p_cond) { _set_enum_conditional(p_cond); }\n")

    fd.write("};\n\n")
    fd.write("#endif\n\n")
    fd.close()


def build_gles3_headers(target, source, env):

    for x in source:
        build_legacygl_header(str(x), include="drivers/gles3/shader_gles3.h", class_suffix="GLES3", output_attribs=True)


def build_gles2_headers(target, source, env):

    for x in source:
        build_legacygl_header(str(x), include="drivers/gles2/shader_gles2.h", class_suffix="GLES2", output_attribs=True, gles2=True)

def make_authors_header(target, source, env):

    sections = ["Project Founders", "Lead Developer", "Project Manager", "Developers"]
    sections_id = ["AUTHORS_FOUNDERS", "AUTHORS_LEAD_DEVELOPERS", "AUTHORS_PROJECT_MANAGERS", "AUTHORS_DEVELOPERS"]

    src = source[0].srcnode().abspath
    dst = target[0].srcnode().abspath
    f = open_utf8(src, "r")
    g = open_utf8(dst, "w")

    g.write("/* THIS FILE IS GENERATED DO NOT EDIT */\n")
    g.write("#ifndef _EDITOR_AUTHORS_H\n")
    g.write("#define _EDITOR_AUTHORS_H\n")

    current_section = ""
    reading = False

    def close_section():
        g.write("\t0\n")
        g.write("};\n")

    for line in f:
        if reading:
            if line.startswith("    "):
                g.write("\t\"" + escape_string(line.strip()) + "\",\n")
                continue
        if line.startswith("## "):
            if reading:
                close_section()
                reading = False
            for i in range(len(sections)):
                if line.strip().endswith(sections[i]):
                    current_section = escape_string(sections_id[i])
                    reading = True
                    g.write("const char *const " + current_section + "[] = {\n")
                    break

    if reading:
        close_section()

    g.write("#endif\n")

    g.close()
    f.close()

def make_donors_header(target, source, env):

    sections = ["Platinum sponsors", "Gold sponsors", "Mini sponsors",
            "Gold donors", "Silver donors", "Bronze donors"]
    sections_id = ["DONORS_SPONSOR_PLAT", "DONORS_SPONSOR_GOLD", "DONORS_SPONSOR_MINI",
            "DONORS_GOLD", "DONORS_SILVER", "DONORS_BRONZE"]

    src = source[0].srcnode().abspath
    dst = target[0].srcnode().abspath
    f = open_utf8(src, "r")
    g = open_utf8(dst, "w")

    g.write("/* THIS FILE IS GENERATED DO NOT EDIT */\n")
    g.write("#ifndef _EDITOR_DONORS_H\n")
    g.write("#define _EDITOR_DONORS_H\n")

    current_section = ""
    reading = False

    def close_section():
        g.write("\t0\n")
        g.write("};\n")

    for line in f:
        if reading >= 0:
            if line.startswith("    "):
                g.write("\t\"" + escape_string(line.strip()) + "\",\n")
                continue
        if line.startswith("## "):
            if reading:
                close_section()
                reading = False
            for i in range(len(sections)):
                if line.strip().endswith(sections[i]):
                    current_section = escape_string(sections_id[i])
                    reading = True
                    g.write("const char *const " + current_section + "[] = {\n")
                    break

    if reading:
        close_section()

    g.write("#endif\n")

    g.close()
    f.close()


def make_license_header(target, source, env):
    src_copyright = source[0].srcnode().abspath
    src_license = source[1].srcnode().abspath
    dst = target[0].srcnode().abspath

    class LicenseReader:
        def __init__(self, license_file):
            self._license_file = license_file
            self.line_num = 0
            self.current = self.next_line()

        def next_line(self):
            line = self._license_file.readline()
            self.line_num += 1
            while line.startswith("#"):
                line = self._license_file.readline()
                self.line_num += 1
            self.current = line
            return line

        def next_tag(self):
            if not ':' in self.current:
                return ('',[])
            tag, line = self.current.split(":", 1)
            lines = [line.strip()]
            while self.next_line() and self.current.startswith(" "):
                lines.append(self.current.strip())
            return (tag, lines)

    from collections import OrderedDict
    projects = OrderedDict()
    license_list = []

    with open_utf8(src_copyright, "r") as copyright_file:
        reader = LicenseReader(copyright_file)
        part = {}
        while reader.current:
            tag, content = reader.next_tag()
            if tag in ("Files", "Copyright", "License"):
                part[tag] = content[:]
            elif tag == "Comment":
                # attach part to named project
                projects[content[0]] = projects.get(content[0], []) + [part]

            if not tag or not reader.current:
                # end of a paragraph start a new part
                if "License" in part and not "Files" in part:
                    # no Files tag in this one, so assume standalone license
                    license_list.append(part["License"])
                part = {}
                reader.next_line()

    data_list = []
    for project in itervalues(projects):
        for part in project:
            part["file_index"] = len(data_list)
            data_list += part["Files"]
            part["copyright_index"] = len(data_list)
            data_list += part["Copyright"]

    with open_utf8(dst, "w") as f:

        f.write("/* THIS FILE IS GENERATED DO NOT EDIT */\n")
        f.write("#ifndef _EDITOR_LICENSE_H\n")
        f.write("#define _EDITOR_LICENSE_H\n")
        f.write("const char *const GODOT_LICENSE_TEXT =")

        with open_utf8(src_license, "r") as license_file:
            for line in license_file:
                escaped_string = escape_string(line.strip())
                f.write("\n\t\t\"" + escaped_string + "\\n\"")
        f.write(";\n\n")

        f.write("struct ComponentCopyrightPart {\n"
                "\tconst char *license;\n"
                "\tconst char *const *files;\n"
                "\tconst char *const *copyright_statements;\n"
                "\tint file_count;\n"
                "\tint copyright_count;\n"
                "};\n\n")

        f.write("struct ComponentCopyright {\n"
                "\tconst char *name;\n"
                "\tconst ComponentCopyrightPart *parts;\n"
                "\tint part_count;\n"
                "};\n\n")

        f.write("const char *const COPYRIGHT_INFO_DATA[] = {\n")
        for line in data_list:
            f.write("\t\"" + escape_string(line) + "\",\n")
        f.write("};\n\n")

        f.write("const ComponentCopyrightPart COPYRIGHT_PROJECT_PARTS[] = {\n")
        part_index = 0
        part_indexes = {}
        for project_name, project in iteritems(projects):
            part_indexes[project_name] = part_index
            for part in project:
                f.write("\t{ \"" + escape_string(part["License"][0]) + "\", "
                        + "&COPYRIGHT_INFO_DATA[" + str(part["file_index"]) + "], "
                        + "&COPYRIGHT_INFO_DATA[" + str(part["copyright_index"]) + "], "
                        + str(len(part["Files"])) + ", "
                        + str(len(part["Copyright"])) + " },\n")
                part_index += 1
        f.write("};\n\n")

        f.write("const int COPYRIGHT_INFO_COUNT = " + str(len(projects)) + ";\n")

        f.write("const ComponentCopyright COPYRIGHT_INFO[] = {\n")
        for project_name, project in iteritems(projects):
            f.write("\t{ \"" + escape_string(project_name) + "\", "
                    + "&COPYRIGHT_PROJECT_PARTS[" + str(part_indexes[project_name]) + "], "
                    + str(len(project)) + " },\n")
        f.write("};\n\n")

        f.write("const int LICENSE_COUNT = " + str(len(license_list)) + ";\n")

        f.write("const char *const LICENSE_NAMES[] = {\n")
        for l in license_list:
            f.write("\t\"" + escape_string(l[0]) + "\",\n")
        f.write("};\n\n")

        f.write("const char *const LICENSE_BODIES[] = {\n\n")
        for l in license_list:
            for line in l[1:]:
                if line == ".":
                    f.write("\t\"\\n\"\n")
                else:
                    f.write("\t\"" + escape_string(line) + "\\n\"\n")
            f.write("\t\"\",\n\n")
        f.write("};\n\n")

        f.write("#endif\n")
def add_module_version_string(self,s):
    self.module_version_string += "." + s

def update_version(module_version_string=""):

    build_name = "custom_build"
    if (os.getenv("BUILD_NAME") != None):
        build_name = os.getenv("BUILD_NAME")
        print("Using custom build name: " + build_name)

    import version

    f = open("core/version_generated.gen.h", "w")
    f.write("#define VERSION_SHORT_NAME \"" + str(version.short_name) + "\"\n")
    f.write("#define VERSION_NAME \"" + str(version.name) + "\"\n")
    f.write("#define VERSION_MAJOR " + str(version.major) + "\n")
    f.write("#define VERSION_MINOR " + str(version.minor) + "\n")
    if (hasattr(version, 'patch')):
        f.write("#define VERSION_PATCH " + str(version.patch) + "\n")
    f.write("#define VERSION_STATUS \"" + str(version.status) + "\"\n")
    f.write("#define VERSION_BUILD \"" + str(build_name) + "\"\n")
    f.write("#define VERSION_MODULE_CONFIG \"" + str(version.module_config) + module_version_string + "\"\n")
    import datetime
    f.write("#define VERSION_YEAR " + str(datetime.datetime.now().year) + "\n")
    f.close()

    fhash = open("core/version_hash.gen.h", "w")
    githash = ""
    if os.path.isfile(".git/HEAD"):
        head = open(".git/HEAD", "r").readline().strip()
        if head.startswith("ref: "):
            head = ".git/" + head[5:]
            if os.path.isfile(head):
                githash = open(head, "r").readline().strip()
        else:
            githash = head
    fhash.write("#define VERSION_HASH \"" + githash + "\"")
    fhash.close()


def parse_cg_file(fname, uniforms, sizes, conditionals):

    import re
    fs = open(fname, "r")
    line = fs.readline()

    while line:

        if re.match(r"^\s*uniform", line):

            res = re.match(r"uniform ([\d\w]*) ([\d\w]*)")
            type = res.groups(1)
            name = res.groups(2)

            uniforms.append(name)

            if (type.find("texobj") != -1):
                sizes.append(1)
            else:
                t = re.match(r"float(\d)x(\d)", type)
                if t:
                    sizes.append(int(t.groups(1)) * int(t.groups(2)))
                else:
                    t = re.match(r"float(\d)", type)
                    sizes.append(int(t.groups(1)))

            if line.find("[branch]") != -1:
                conditionals.append(name)

        line = fs.readline()

    fs.close()


import glob


def detect_modules():

    module_list = []
    includes_cpp = ""
    register_cpp = ""
    unregister_cpp = ""

    files = glob.glob("modules/*")
    files.sort()  # so register_module_types does not change that often, and also plugins are registered in alphabetic order
    for x in files:
        if (not os.path.isdir(x)):
            continue
        if (not os.path.exists(x + "/config.py")):
            continue
        x = x.replace("modules/", "")  # rest of world
        x = x.replace("modules\\", "")  # win32
        module_list.append(x)
        try:
            with open("modules/" + x + "/register_types.h"):
                includes_cpp += '#include "modules/' + x + '/register_types.h"\n'
                register_cpp += '#ifdef MODULE_' + x.upper() + '_ENABLED\n'
                register_cpp += '\tregister_' + x + '_types();\n'
                register_cpp += '#endif\n'
                unregister_cpp += '#ifdef MODULE_' + x.upper() + '_ENABLED\n'
                unregister_cpp += '\tunregister_' + x + '_types();\n'
                unregister_cpp += '#endif\n'
        except IOError:
            pass

    modules_cpp = """
// modules.cpp - THIS FILE IS GENERATED, DO NOT EDIT!!!!!!!
#include "register_module_types.h"

""" + includes_cpp + """

void register_module_types() {
""" + register_cpp + """
}

void unregister_module_types() {
""" + unregister_cpp + """
}
"""

    with open("modules/register_module_types.gen.cpp", "w") as f:
        f.write(modules_cpp)

    return module_list


def win32_spawn(sh, escape, cmd, args, env):
    import subprocess
    newargs = ' '.join(args[1:])
    cmdline = cmd + " " + newargs
    startupinfo = subprocess.STARTUPINFO()
    for e in env:
        if type(env[e]) != type(""):
            env[e] = str(env[e])
    proc = subprocess.Popen(cmdline, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE, startupinfo=startupinfo, shell=False, env=env)
    data, err = proc.communicate()
    rv = proc.wait()
    if rv:
        print("=====")
        print(err)
        print("=====")
    return rv

"""
def win32_spawn(sh, escape, cmd, args, spawnenv):
	import win32file
	import win32event
	import win32process
	import win32security
	for var in spawnenv:
		spawnenv[var] = spawnenv[var].encode('ascii', 'replace')

	sAttrs = win32security.SECURITY_ATTRIBUTES()
	StartupInfo = win32process.STARTUPINFO()
	newargs = ' '.join(map(escape, args[1:]))
	cmdline = cmd + " " + newargs

	# check for any special operating system commands
	if cmd == 'del':
		for arg in args[1:]:
			win32file.DeleteFile(arg)
		exit_code = 0
	else:
		# otherwise execute the command.
		hProcess, hThread, dwPid, dwTid = win32process.CreateProcess(None, cmdline, None, None, 1, 0, spawnenv, None, StartupInfo)
		win32event.WaitForSingleObject(hProcess, win32event.INFINITE)
		exit_code = win32process.GetExitCodeProcess(hProcess)
		win32file.CloseHandle(hProcess);
		win32file.CloseHandle(hThread);
	return exit_code
"""

def android_add_flat_dir(self, dir):
    if (dir not in self.android_flat_dirs):
        self.android_flat_dirs.append(dir)

def android_add_maven_repository(self, url):
    if (url not in self.android_maven_repos):
        self.android_maven_repos.append(url)

def android_add_dependency(self, depline):
    if (depline not in self.android_dependencies):
        self.android_dependencies.append(depline)

def android_add_java_dir(self, subpath):
    base_path = self.Dir(".").abspath + "/modules/" + self.current_module + "/" + subpath
    if (base_path not in self.android_java_dirs):
        self.android_java_dirs.append(base_path)

def android_add_res_dir(self, subpath):
    base_path = self.Dir(".").abspath + "/modules/" + self.current_module + "/" + subpath
    if (base_path not in self.android_res_dirs):
        self.android_res_dirs.append(base_path)

def android_add_asset_dir(self, subpath):
    base_path = self.Dir(".").abspath + "/modules/" + self.current_module + "/" + subpath
    if (base_path not in self.android_asset_dirs):
        self.android_asset_dirs.append(base_path)

def android_add_aidl_dir(self, subpath):
    base_path = self.Dir(".").abspath + "/modules/" + self.current_module + "/" + subpath
    if (base_path not in self.android_aidl_dirs):
        self.android_aidl_dirs.append(base_path)

def android_add_jni_dir(self, subpath):
    base_path = self.Dir(".").abspath + "/modules/" + self.current_module + "/" + subpath
    if (base_path not in self.android_jni_dirs):
        self.android_jni_dirs.append(base_path)

def android_add_gradle_plugin(self, plugin):
    if (plugin not in self.android_gradle_plugins):
        self.android_gradle_plugins.append(plugin)

def android_add_gradle_classpath(self, classpath):
    if (classpath not in self.android_gradle_classpath):
        self.android_gradle_classpath.append(classpath)

def android_add_default_config(self, config):
    if (config not in self.android_default_config):
        self.android_default_config.append(config)

def android_add_to_manifest(self, file):
    base_path = self.Dir(".").abspath + "/modules/" + self.current_module + "/" + file
    with open(base_path, "r") as f:
        self.android_manifest_chunk += f.read()

def android_add_to_permissions(self, file):
    base_path = self.Dir(".").abspath + "/modules/" + self.current_module + "/" + file
    with open(base_path, "r") as f:
        self.android_permission_chunk += f.read()

def android_add_to_attributes(self, file):
    base_path = self.Dir(".").abspath + "/modules/" + self.current_module + "/" + file
    with open(base_path, "r") as f:
        self.android_appattributes_chunk += f.read()

def disable_module(self):
    self.disabled_modules.append(self.current_module)

def use_windows_spawn_fix(self, platform=None):

    if (os.name != "nt"):
        return  # not needed, only for windows

    # On Windows, due to the limited command line length, when creating a static library
    # from a very high number of objects SCons will invoke "ar" once per object file;
    # that makes object files with same names to be overwritten so the last wins and
    # the library looses symbols defined by overwritten objects.
    # By enabling quick append instead of the default mode (replacing), libraries will
    # got built correctly regardless the invocation strategy.
    # Furthermore, since SCons will rebuild the library from scratch when an object file
    # changes, no multiple versions of the same object file will be present.
    self.Replace(ARFLAGS='q')

    import subprocess

    def mySubProcess(cmdline, env):

        startupinfo = subprocess.STARTUPINFO()
        startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        proc = subprocess.Popen(cmdline, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, startupinfo=startupinfo, shell=False, env=env)
        data, err = proc.communicate()
        rv = proc.wait()
        if rv:
            print("=====")
            print(err)
            print("=====")
        return rv

    def mySpawn(sh, escape, cmd, args, env):

        newargs = ' '.join(args[1:])
        cmdline = cmd + " " + newargs

        rv = 0
        env = {str(key): str(value) for key, value in iteritems(env)}
        if len(cmdline) > 32000 and cmd.endswith("ar"):
            cmdline = cmd + " " + args[1] + " " + args[2] + " "
            for i in range(3, len(args)):
                rv = mySubProcess(cmdline + args[i], env)
                if rv:
                    break
        else:
            rv = mySubProcess(cmdline, env)

        return rv

    self['SPAWN'] = mySpawn


def split_lib(self, libname, src_list = None, env_lib = None):
    import string
    env = self

    num = 0
    cur_base = ""
    max_src = 64
    list = []
    lib_list = []

    if src_list == None:
        src_list = getattr(env, libname + "_sources")

    if type(env_lib) == type(None):
        env_lib = env

    for f in src_list:
        fname = ""
        if type(f) == type(""):
            fname = env.File(f).path
        else:
            fname = env.File(f)[0].path
        fname = fname.replace("\\", "/")
        base = string.join(fname.split("/")[:2], "/")
        if base != cur_base and len(list) > max_src:
            if num > 0:
                lib = env_lib.add_library(libname + str(num), list)
                lib_list.append(lib)
                list = []
            num = num + 1
        cur_base = base
        list.append(f)

    lib = env_lib.add_library(libname + str(num), list)
    lib_list.append(lib)

    if len(lib_list) > 0:
        import os, sys
        if os.name == 'posix' and sys.platform == 'msys':
            env.Replace(ARFLAGS=['rcsT'])
            lib = env_lib.add_library(libname + "_collated", lib_list)
            lib_list = [lib]

    lib_base = []
    env_lib.add_source_files(lib_base, "*.cpp")
    lib = env_lib.add_library(libname, lib_base)
    lib_list.insert(0, lib)

    env.Prepend(LIBS=lib_list)


def save_active_platforms(apnames, ap):

    for x in ap:
        names = ['logo']
        if os.path.isfile(x + "/run_icon.png"):
            names.append('run_icon')

        for name in names:
            pngf = open(x + "/" + name + ".png", "rb")
            b = pngf.read(1)
            str = " /* AUTOGENERATED FILE, DO NOT EDIT */ \n"
            str += " static const unsigned char _" + x[9:] + "_" + name + "[]={"
            while(len(b) == 1):
                str += hex(ord(b))
                b = pngf.read(1)
                if (len(b) == 1):
                    str += ","

            str += "};\n"

            pngf.close()

            wf = x + "/" + name + ".gen.h"
            with open(wf, "w") as pngw:
                pngw.write(str)


def no_verbose(sys, env):

    colors = {}

    # Colors are disabled in non-TTY environments such as pipes. This means
    # that if output is redirected to a file, it will not contain color codes
    if sys.stdout.isatty():
        colors['cyan'] = '\033[96m'
        colors['purple'] = '\033[95m'
        colors['blue'] = '\033[94m'
        colors['green'] = '\033[92m'
        colors['yellow'] = '\033[93m'
        colors['red'] = '\033[91m'
        colors['end'] = '\033[0m'
    else:
        colors['cyan'] = ''
        colors['purple'] = ''
        colors['blue'] = ''
        colors['green'] = ''
        colors['yellow'] = ''
        colors['red'] = ''
        colors['end'] = ''

    compile_source_message = '%sCompiling %s==> %s$SOURCE%s' % (colors['blue'], colors['purple'], colors['yellow'], colors['end'])
    java_compile_source_message = '%sCompiling %s==> %s$SOURCE%s' % (colors['blue'], colors['purple'], colors['yellow'], colors['end'])
    compile_shared_source_message = '%sCompiling shared %s==> %s$SOURCE%s' % (colors['blue'], colors['purple'], colors['yellow'], colors['end'])
    link_program_message = '%sLinking Program        %s==> %s$TARGET%s' % (colors['red'], colors['purple'], colors['yellow'], colors['end'])
    link_library_message = '%sLinking Static Library %s==> %s$TARGET%s' % (colors['red'], colors['purple'], colors['yellow'], colors['end'])
    ranlib_library_message = '%sRanlib Library         %s==> %s$TARGET%s' % (colors['red'], colors['purple'], colors['yellow'], colors['end'])
    link_shared_library_message = '%sLinking Shared Library %s==> %s$TARGET%s' % (colors['red'], colors['purple'], colors['yellow'], colors['end'])
    java_library_message = '%sCreating Java Archive  %s==> %s$TARGET%s' % (colors['red'], colors['purple'], colors['yellow'], colors['end'])

    env.Append(CXXCOMSTR=[compile_source_message])
    env.Append(CCCOMSTR=[compile_source_message])
    env.Append(SHCCCOMSTR=[compile_shared_source_message])
    env.Append(SHCXXCOMSTR=[compile_shared_source_message])
    env.Append(ARCOMSTR=[link_library_message])
    env.Append(RANLIBCOMSTR=[ranlib_library_message])
    env.Append(SHLINKCOMSTR=[link_shared_library_message])
    env.Append(LINKCOMSTR=[link_program_message])
    env.Append(JARCOMSTR=[java_library_message])
    env.Append(JAVACCOMSTR=[java_compile_source_message])


def detect_visual_c_compiler_version(tools_env):
    # tools_env is the variable scons uses to call tools that execute tasks, SCons's env['ENV'] that executes tasks...
    # (see the SCons documentation for more information on what it does)...
    # in order for this function to be well encapsulated i choose to force it to receive SCons's TOOLS env (env['ENV']
    # and not scons setup environment (env)... so make sure you call the right environment on it or it will fail to detect
    # the proper vc version that will be called

    # There is no flag to give to visual c compilers to set the architecture, ie scons bits argument (32,64,ARM etc)
    # There are many different cl.exe files that are run, and each one compiles & links to a different architecture
    # As far as I know, the only way to figure out what compiler will be run when Scons calls cl.exe via Program()
    # is to check the PATH variable and figure out which one will be called first. Code below does that and returns:
    # the following string values:

    # ""              Compiler not detected
    # "amd64"         Native 64 bit compiler
    # "amd64_x86"     64 bit Cross Compiler for 32 bit
    # "x86"           Native 32 bit compiler
    # "x86_amd64"     32 bit Cross Compiler for 64 bit

    # There are other architectures, but Godot does not support them currently, so this function does not detect arm/amd64_arm
    # and similar architectures/compilers

    # Set chosen compiler to "not detected"
    vc_chosen_compiler_index = -1
    vc_chosen_compiler_str = ""

    # Start with Pre VS 2017 checks which uses VCINSTALLDIR:
    if 'VCINSTALLDIR' in tools_env:
        # print("Checking VCINSTALLDIR")

        # find() works with -1 so big ifs below are needed... the simplest solution, in fact
        # First test if amd64 and amd64_x86 compilers are present in the path
        vc_amd64_compiler_detection_index = tools_env["PATH"].find(tools_env["VCINSTALLDIR"] + "BIN\\amd64;")
        if(vc_amd64_compiler_detection_index > -1):
            vc_chosen_compiler_index = vc_amd64_compiler_detection_index
            vc_chosen_compiler_str = "amd64"

        vc_amd64_x86_compiler_detection_index = tools_env["PATH"].find(tools_env["VCINSTALLDIR"] + "BIN\\amd64_x86;")
        if(vc_amd64_x86_compiler_detection_index > -1
           and (vc_chosen_compiler_index == -1
                or vc_chosen_compiler_index > vc_amd64_x86_compiler_detection_index)):
            vc_chosen_compiler_index = vc_amd64_x86_compiler_detection_index
            vc_chosen_compiler_str = "amd64_x86"

        # Now check the 32 bit compilers
        vc_x86_compiler_detection_index = tools_env["PATH"].find(tools_env["VCINSTALLDIR"] + "BIN;")
        if(vc_x86_compiler_detection_index > -1
           and (vc_chosen_compiler_index == -1
                or vc_chosen_compiler_index > vc_x86_compiler_detection_index)):
            vc_chosen_compiler_index = vc_x86_compiler_detection_index
            vc_chosen_compiler_str = "x86"

        vc_x86_amd64_compiler_detection_index = tools_env["PATH"].find(tools_env['VCINSTALLDIR'] + "BIN\\x86_amd64;")
        if(vc_x86_amd64_compiler_detection_index > -1
           and (vc_chosen_compiler_index == -1
                or vc_chosen_compiler_index > vc_x86_amd64_compiler_detection_index)):
            vc_chosen_compiler_index = vc_x86_amd64_compiler_detection_index
            vc_chosen_compiler_str = "x86_amd64"

    # and for VS 2017 and newer we check VCTOOLSINSTALLDIR:
    if 'VCTOOLSINSTALLDIR' in tools_env:

        # Newer versions have a different path available
        vc_amd64_compiler_detection_index = tools_env["PATH"].upper().find(tools_env['VCTOOLSINSTALLDIR'].upper() + "BIN\\HOSTX64\\X64;")
        if(vc_amd64_compiler_detection_index > -1):
            vc_chosen_compiler_index = vc_amd64_compiler_detection_index
            vc_chosen_compiler_str = "amd64"

        vc_amd64_x86_compiler_detection_index = tools_env["PATH"].upper().find(tools_env['VCTOOLSINSTALLDIR'].upper() + "BIN\\HOSTX64\\X86;")
        if(vc_amd64_x86_compiler_detection_index > -1
           and (vc_chosen_compiler_index == -1
                or vc_chosen_compiler_index > vc_amd64_x86_compiler_detection_index)):
            vc_chosen_compiler_index = vc_amd64_x86_compiler_detection_index
            vc_chosen_compiler_str = "amd64_x86"

        vc_x86_compiler_detection_index = tools_env["PATH"].upper().find(tools_env['VCTOOLSINSTALLDIR'].upper() + "BIN\\HOSTX86\\X86;")
        if(vc_x86_compiler_detection_index > -1
           and (vc_chosen_compiler_index == -1
                or vc_chosen_compiler_index > vc_x86_compiler_detection_index)):
            vc_chosen_compiler_index = vc_x86_compiler_detection_index
            vc_chosen_compiler_str = "x86"

        vc_x86_amd64_compiler_detection_index = tools_env["PATH"].upper().find(tools_env['VCTOOLSINSTALLDIR'].upper() + "BIN\\HOSTX86\\X64;")
        if(vc_x86_amd64_compiler_detection_index > -1
           and (vc_chosen_compiler_index == -1
                or vc_chosen_compiler_index > vc_x86_amd64_compiler_detection_index)):
            vc_chosen_compiler_index = vc_x86_amd64_compiler_detection_index
            vc_chosen_compiler_str = "x86_amd64"

    return vc_chosen_compiler_str

def find_visual_c_batch_file(env):
    from  SCons.Tool.MSCommon.vc import get_default_version, get_host_target, find_batch_file

    version = get_default_version(env)
    (host_platform, target_platform,req_target_platform) = get_host_target(env)
    return find_batch_file(env, version, host_platform, target_platform)[0]

def generate_cpp_hint_file(filename):
    import os.path
    if os.path.isfile(filename):
        # Don't overwrite an existing hint file since the user may have customized it.
        pass
    else:
        try:
            with open(filename, "w") as fd:
                fd.write("#define GDCLASS(m_class, m_inherits)\n")
        except IOError:
            print("Could not write cpp.hint file.")

def generate_vs_project(env, num_jobs):
    batch_file = find_visual_c_batch_file(env)
    if batch_file:
        def build_commandline(commands):
            common_build_prefix = ['cmd /V /C set "plat=$(PlatformTarget)"',
                                    '(if "$(PlatformTarget)"=="x64" (set "plat=x86_amd64"))',
                                    'set "tools=yes"',
                                    '(if "$(Configuration)"=="release" (set "tools=no"))',
                                    'call "' + batch_file + '" !plat!']

            result = " ^& ".join(common_build_prefix + [commands])
            return result

        env.AddToVSProject(env.core_sources)
        env.AddToVSProject(env.main_sources)
        env.AddToVSProject(env.modules_sources)
        env.AddToVSProject(env.scene_sources)
        env.AddToVSProject(env.servers_sources)
        env.AddToVSProject(env.editor_sources)

        # windows allows us to have spaces in paths, so we need
        # to double quote off the directory. However, the path ends
        # in a backslash, so we need to remove this, lest it escape the
        # last double quote off, confusing MSBuild
        env['MSVSBUILDCOM'] = build_commandline('scons --directory="$(ProjectDir.TrimEnd(\'\\\'))" platform=windows progress=no target=$(Configuration) tools=!tools! -j' + str(num_jobs))
        env['MSVSREBUILDCOM'] = build_commandline('scons --directory="$(ProjectDir.TrimEnd(\'\\\'))" platform=windows progress=no target=$(Configuration) tools=!tools! vsproj=yes -j' + str(num_jobs))
        env['MSVSCLEANCOM'] = build_commandline('scons --directory="$(ProjectDir.TrimEnd(\'\\\'))" --clean platform=windows progress=no target=$(Configuration) tools=!tools! -j' + str(num_jobs))

        # This version information (Win32, x64, Debug, Release, Release_Debug seems to be
        # required for Visual Studio to understand that it needs to generate an NMAKE
        # project. Do not modify without knowing what you are doing.
        debug_variants = ['debug|Win32'] + ['debug|x64']
        release_variants = ['release|Win32'] + ['release|x64']
        release_debug_variants = ['release_debug|Win32'] + ['release_debug|x64']
        variants = debug_variants + release_variants + release_debug_variants
        debug_targets = ['bin\\godot.windows.tools.32.exe'] + ['bin\\godot.windows.tools.64.exe']
        release_targets = ['bin\\godot.windows.opt.32.exe'] + ['bin\\godot.windows.opt.64.exe']
        release_debug_targets = ['bin\\godot.windows.opt.tools.32.exe'] + ['bin\\godot.windows.opt.tools.64.exe']
        targets = debug_targets + release_targets + release_debug_targets
        msvproj = env.MSVSProject(target=['#godot' + env['MSVSPROJECTSUFFIX']],
                                    incs=env.vs_incs,
                                    srcs=env.vs_srcs,
                                    runfile=targets,
                                    buildtarget=targets,
                                    auto_build_solution=1,
                                    variant=variants)
    else:
        print("Could not locate Visual Studio batch file for setting up the build environment. Not generating VS project.")

def precious_program(env, program, sources, **args):
    program = env.ProgramOriginal(program, sources, **args)
    env.Precious(program)
    return program

def add_shared_library(env, name, sources, **args):
    library = env.SharedLibrary(name, sources, **args)
    env.NoCache(library)
    return library

def add_library(env, name, sources, **args):
    library = env.Library(name, sources, **args)
    env.NoCache(library)
    return library

def add_program(env, name, sources, **args):
    program = env.Program(name, sources, **args)
    env.NoCache(program)
    return program

def CommandNoCache(env, target, sources, command, **args):
    result = env.Command(target, sources, command, **args)
    env.NoCache(result)
    return result
