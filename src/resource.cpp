#include "resource.hpp"

void Resource::generate()
{
    YAML::Node file = get_yaml("deus-config.yml");
    readFile(file);
    check_directories();
    writeModel();
    writeValidator();
    writePolicy();
    writeRoutes();
    writeController();
    writeSeeder();
    writeTests();
    writeMigration();
}

YAML::Node Resource::get_yaml(const std::string &path)
{
    try {
        return YAML::LoadFile(path);
    } catch(const YAML::BadFile &e) {
        throw std::runtime_error("Could not load file \"" + path + "\":\n" + e.what());
    }
}

void Resource::check_directories()
{
    std::vector<std::string_view> dirs = {"app", "app/Controllers", "app/Controllers/Http", "app/Models",
    "app/Validators", _validator_folder, "app/Policies", "start", "start/routes", "database", "database/seeders", "database/migrations", "test"};

    for (auto &dir : dirs) {
        if (std::filesystem::is_directory(dir) && !_override)
            return;
        std::filesystem::create_directory(dir);
    }
}

void Resource::readFile(YAML::Node &file)
{
    if (!file["crud-name"])
        throw std::runtime_error("Crud name value is not set");

    const std::string crudName = file["crud-name"].as<std::string>();
    setCrudName(crudName);

    const YAML::Node& models = file["models"];
    for (std::size_t i = 0; i < models.size(); i++) {
        const YAML::Node model = models[i];
        for(auto j = model.begin(); j != model.end(); j++) {
            _column.emplace(j->first.as<std::string>(), j->second.as<std::string>());
        }
    }

    const YAML::Node& foreign = file["foreign-keys"];
    for (std::size_t i = 0; i < foreign.size(); i++) {
        const YAML::Node key = foreign[i];
        for(auto j = key.begin(); j != key.end(); j++) {
            foreign_keys.emplace(std::make_pair(j->first.as<std::string>(), j->second.as<std::string>()));
        }
    }

    const YAML::Node& swivel = file["swivel-tables"];
    for (std::size_t i = 0; i < swivel.size(); i++) {
        const YAML::Node key = swivel[i];
        for(auto j = key.begin(); j != key.end(); j++) {
            swivel_tables.emplace(std::make_pair(j->first.as<std::string>(), j->second.as<std::string>()));
        }
    }

    const YAML::Node &optional = file["body-validator-optional-values"];
    for(YAML::const_iterator it = optional.begin(); it != optional.end(); it++) {
        const YAML::Node &opt = *it;
        validator_optional.push_back(opt.as<std::string>());
    }

    const YAML::Node &controller = file["controller"];
    for (std::size_t i = 0; i < controller.size(); i++) {
        const YAML::Node route = controller[i];
        if (controller[i]["crud"]) {
            route_crud = controller[i]["crud"].as<bool>();
        }
        if (controller[i]["additionals"]) {
            for (std::size_t j = 0; j < controller[i]["additionals"].size(); j++) {
                additional_routes.emplace(controller[i]["additionals"][j]["routes"].as<std::string>(), controller[i]["additionals"][j]["http"].as<std::string>());
            }
        }
    }

    if (!file["policies"])
        throw std::runtime_error("Policies value is not set");
    const bool policies = file["policies"].as<bool>();
    setPolicies(policies);

    if (!file["automated-routes-testing"])
        throw "Tests choice value is not set";

    const bool tests = file["automated-routes-testing"].as<bool>();
    setTests(tests);
}

void Resource::checkFiles(std::filesystem::path file, std::filesystem::path path)
{
    if (std::filesystem::exists(path / file) && !_override)
        throw std::runtime_error("file " + std::string(path / file) + " already exists");
}

void Resource::setCrudName(std::string _var) {
    std::string crudLower = _var;
    std::for_each(crudLower.begin(), crudLower.end(), [](char & c){
        c = ::tolower(c);
    });
    _crud_name = _var;
    _crud_lower = crudLower;
    _validator_folder = "app/Validators/" + _var;
}

void Resource::setPolicies(bool _var) {
    _policies = _var;
}

void Resource::setTests(bool _var) {
    _tests = _var;
}

void Resource::writeController()
{
    std::string headers =
    "import { HttpContextContract } from '@ioc:Adonis/Core/HttpContext'\nimport Env from '@ioc:Adonis/Core/Env'\nimport { Filters } from 'App/Services/Filters'\nimport Logger from '@ioc:Adonis/Core/Logger'\nimport " + _crud_name + "Validator from 'App/Validators/" + _crud_name + "Validator'\nimport " + _crud_name + " from 'App/Models/" + _crud_name + "'";
    std::string fileName = _crud_name + "sController.ts";
    std::string ControllerInit = "\n\nexport default class " + _crud_name + "Controller {\n\tpublic initQuery() {\n\t\treturn " + _crud_name + ".query()\n\t}";
    std::string ControllerGetAll = "\n\n\tpublic async index({ response, params, request }: HttpContextContract) {\n\t\ttry {\n\t\t\tconst details = await request.validate(" + _crud_name + "Validator)\n\t\t\treturn\n\t\t} catch (e) {\n\t\t\tLogger.error({ err: Error }, e)\n\t\t\treturn response.abort({ errors: [{ message: '" + _crud_name + " Index error' }] })\n\t\t}\n\t}";
    std::string ControllerGet = "\n\n\tpublic async show({ response, params, request }: HttpContextContract) {\n\t\ttry {\n\t\t\tconst details = await request.validate(" + _crud_name + "Validator)\n\t\t\treturn\n\t\t} catch (e) {\n\t\t\tLogger.error({ err: Error }, e)\n\t\t\treturn response.abort({ errors: [{ message: '" + _crud_name + " Show error' }] })\n\t\t}\n\t}";
    std::string ControllerPost = "\n\n\tpublic async store({ response, params, request }: HttpContextContract) {\n\t\ttry {\n\t\t\tconst details = await request.validate(" + _crud_name + "Validator)\n\t\t\treturn\n\t\t} catch (e) {\n\t\t\tLogger.error({ err: Error }, e)\n\t\t\treturn response.abort({ errors: [{ message: '" + _crud_name + " Store error' }] })\n\t\t}\n\t}";
    std::string ControllerUpdate = "\n\n\tpublic async update({ response, params, request }: HttpContextContract) {\n\t\ttry {\n\t\t\tconst details = await request.validate(" + _crud_name + "Validator)\n\t\t\treturn\n\t\t} catch (e) {\n\t\t\tLogger.error({ err: Error }, e)\n\t\t\treturn response.abort({ errors: [{ message: '" + _crud_name + " Update error' }] })\n\t\t}\n\t}";
    std::string ControllerArchive = "\n\n\tpublic async archive({ response, params, request }: HttpContextContract) {\n\t\ttry {\n\t\t\tconst details = await request.validate(" + _crud_name + "Validator)\n\t\t\treturn\n\t\t} catch (e) {\n\t\t\tLogger.error({ err: Error }, e)\n\t\t\treturn response.abort({ errors: [{ message: '" + _crud_name + " Archive error' }] })\n\t\t}\n\t}";
    std::string ControllerDelete = "\n\n\tpublic async destroy({ response, params, request }: HttpContextContract) {\n\t\ttry {\n\t\t\tconst details = await request.validate(" + _crud_name + "Validator)\n\t\t\treturn\n\t\t} catch (e) {\n\t\t\tLogger.error({ err: Error }, e)\n\t\t\treturn response.abort({ errors: [{ message: '" + _crud_name + " Delete error' }] })\n\t\t}\n\t}";
    std::string ControllerEnd =  "\n}\n";

    checkFiles(fileName, "app/Controllers/Http");
    std::ofstream controller("app/Controllers/Http/" + fileName);
    controller << headers << ControllerInit;
    if (route_crud)
        controller << ControllerGetAll << ControllerGet << ControllerPost << ControllerUpdate << ControllerArchive << ControllerDelete;
    controller << ControllerEnd;
}

void Resource::writeModel()
{
    std::string headers =
    "import { DateTime } from 'luxon'\nimport { BaseModel, column, belongsTo, BelongsTo, manyToMany, ManyToMany } from '@ioc:Adonis/Lucid/Orm'";
    std::string fileName = _crud_name + ".ts";
    std::string classInit = "\n\nexport default class " + _crud_name + " extends BaseModel {";
    std::string classEnd = "}\n";
    std::string baseFields = "\n\t@column({ isPrimary: true })\n\tpublic id: number\n";

    checkFiles(fileName, "app/Models");

    std::ofstream model("app/Models/" + fileName);
    model << headers;
    if (!foreign_keys.empty()) {
        for (std::map<std::string, std::string>::iterator it = foreign_keys.begin(); it != foreign_keys.end(); it++) {
            model << "\nimport " + it->second + " from 'App/Models/" + it->second + "'";
        }
    }
    model << classInit << baseFields;
    for (std::map<std::string, std::string>::iterator it = _column.begin(); it != _column.end(); it++) {
        model << "\n\t@column()\n\tpublic " + it->first + ": " + it->second + "\n";
    }
    model << "\n\t@column.dateTime({ autoCreate: true })\n\tpublic createdAt: DateTime\n\n\t@column.dateTime({ autoCreate: true, autoUpdate: true })\n\tpublic updatedAt: DateTime\n\n";
    if (!foreign_keys.empty()) {
        for (std::map<std::string, std::string>::iterator it = foreign_keys.begin(); it != foreign_keys.end(); it++) {
            std::string secondLower = it->second;
            std::for_each(secondLower.begin(), secondLower.end(), [](char & c){
                c = ::tolower(c);
            });
            model << "\t@belongsTo(() => " + it->second + ", {\n\t\tforeignKey: '" + it->first + "',\n\t})\n\tpublic " + secondLower << ": BelongsTo<typeof " + it->second + ">\n";
        }
    }
    if (!swivel_tables.empty()) {
        for (std::map<std::string, std::string>::iterator it = swivel_tables.begin(); it != swivel_tables.end(); it++) {
            std::string secondLower = it->second;
            std::for_each(secondLower.begin(), secondLower.end(), [](char & c){
                c = ::tolower(c);
            });
            model << "\n\t@manyToMany(() => " + it->second + ")\n\tpublic " + it->first + ": ManyToMany<typeof " + it->second << ">\n";
        }
    }
    model << classEnd;
}

void Resource::writePolicy()
{
    std::string headers = "import { BasePolicy } from '@ioc:Adonis/Addons/Bouncer'\nimport " + _crud_name + " from 'App/Models/" + _crud_name + "'";
    std::string fileName = _crud_name + "Policy.ts";
    std::string hasAccess = "\n\nasync function hasAccess() {\n\treturn await " + _crud_name + ".query()\n}\n\n";
    std::string classWrite = "export default class " + _crud_name + "Policy extends BasePolicy {\n\tpublic async before() {\n\t\treturn\n\t}\n\n\tpublic async index() {\n\t\treturn\n\t}\n\n\tpublic async show() {\n\t\treturn\n\t}\n\n\tpublic async store() {\n\t\treturn\n\t}\n\n\tpublic async update() {\n\t\treturn\n\t}\n\n\tpublic async destroy() {\n\t\treturn\n\t}\n}\n";

    checkFiles(fileName, "app/Policies");
    if (_policies) {
        std::ofstream policy("app/Policies/" + fileName);
        policy << headers << hasAccess << classWrite;
    }
}

void Resource::writeValidator()
{
    std::string headers =
    "import { schema, rules } from '@ioc:Adonis/Core/Validator'\nimport { HttpContextContract } from '@ioc:Adonis/Core/HttpContext'";
    std::string fileName = _crud_name + "Validator.ts";
    std::string classInit = "\n\nexport default class " + _crud_name + "Validator {\n\tconstructor(protected ctx: HttpContextContract) {}\n\n\tpublic schema = schema.create({\n";
    std::string classEnd =  "\t})\n\n\tpublic messages = {}\n}\n";
    std::string trim = "{}";

    checkFiles(fileName, "app/Validators");

    std::ofstream validator("app/Validators/" + _crud_name + "/" + fileName);
    validator << headers << classInit;
    for (std::map<std::string, std::string>::iterator it = _column.begin(); it != _column.end(); it++) {
        std::string opt = "";
        for (std::size_t i = 0; i < validator_optional.size(); i++) {
            if (validator_optional[i] == it->first)
                opt = ".optional";
        }
        if (it->second.compare("string") == 0) {
            trim = "{ trim: true }";
        }
        validator << "\t\t" + it->first + ": schema." + it->second + opt + "(" + trim + ", []),\n";
        trim = "{}";
    }
    validator << classEnd;
}

void Resource::writeRoutes()
{
    std::string headers =
    "import Route from '@ioc:Adonis/Core/Route'\n\n";
    std::string fileName = _crud_lower + "s.ts";
    std::string RoutesInit = "/**\n * " + _crud_name + "\n */\nRoute.group(() => {\n\t";
    std::string RoutesEnd =  "}).middleware(['auth'])\n";

    checkFiles(fileName, "start/routes");
    std::ofstream routes("start/routes/" + fileName);
    routes << headers << RoutesInit;
    if (route_crud)
        routes << "Route.resource('" + _crud_lower + "', '" + _crud_name + "Controller')\n";
    for (std::map<std::string, std::string>::iterator it = additional_routes.begin(); it != additional_routes.end(); it++)
        routes << "\tRoute." + it->second + "('" + it->first + "', '" + _crud_name + "Controller')\n";
    routes << RoutesEnd;

    std::ofstream routesIndex;
    routesIndex.open("start/routes.ts", std::ios_base::app);
    routesIndex << "import './routes/" + _crud_lower + "'\n";
}

void Resource::writeSeeder()
{
    std::string headers =
    "import BaseSeeder from '@ioc:Adonis/Lucid/Seeder'\nimport " + _crud_name + " from 'App/Models/" + _crud_name + "'";
    std::string fileName = _crud_name + ".ts";
    std::string classInit = "\n\nexport default class " + _crud_name + "Seeder extends BaseSeeder {\n\tpublic async run() {\n\t\t";
    std::string fillInit = "const " + _crud_lower + " = await " + _crud_name + ".create({\n";
    std::string classEnd =  "\t\t})\n\t}\n}\n";

    checkFiles(fileName, "database/seeders");
    if (_tests) {
        std::ofstream seeder("database/seeders/" + fileName);
        seeder << headers << classInit << fillInit;
        for (std::map<std::string, std::string>::iterator it = _column.begin(); it != _column.end(); it++) {
            std::string str = "other_types";
            int num = 0;
            bool bl = false;
            if (it->second == "number")
                seeder << "\t\t\t" << it->first << ": " << num << ",\n";
            else if (it->second == "Boolean" || it->second == "boolean" || it->second == "bool")
                seeder << "\t\t\t" << it->first << ": " << bl << ",\n";
            else
                seeder << "\t\t\t" << it->first << ": '" << str << "',\n";
        }
        seeder << classEnd;
    }
}

void Resource::writeTests()
{
    std::string headers = "import test from 'japa'\nimport supertest from 'supertest'";
    std::string fileName = _crud_name + ".spec.ts";
    std::string consts = "\n\nconst BASE_URL = `http://${process.env.HOST}:${process.env.PORT}`\n\n";
    std::string testsInit = "test.group('test @Work API " + _crud_name + " controller', async () => {";
    std::string loginTests = "\n\t/**\n\t * Login\n\t */\n\ttest.group('test @Work login', async () => {\n\t})\n";
    std::string crudTests = "\n\t/**\n\t * " + _crud_name + "\n\t */\n\ttest.group('test @Work " + _crud_lower + "', async () => {\n\t})\n";
    std::string testsEnd =  "})\n";

    checkFiles(fileName, "test");
    if (_tests) {
        std::ofstream tests("test/" + fileName);
        tests << headers << consts << testsInit << loginTests << crudTests << testsEnd;
    }
}

void Resource::writeMigration()
{
    std::string cmd = "node ace make:migration " + _crud_name;
    const char* _cmd = cmd.c_str();
    system(_cmd);
}
