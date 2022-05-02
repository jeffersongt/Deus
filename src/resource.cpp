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
    std::vector<std::string_view> dirs = {"app", "app/Controllers", "app/Controllers/Http", "app/Controllers/Http/Admin", "app/Models",
    "app/Validators", "app/Policies", "start", "start/routes", "database", "database/seeders", "database/migrations", "test"};

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

    const YAML::Node& swivel = file["pivot-tables"];
    for (std::size_t i = 0; i < swivel.size(); i++) {
        const YAML::Node key = swivel[i];
        for(auto j = key.begin(); j != key.end(); j++) {
            pivot_tables.emplace(std::make_pair(j->first.as<std::string>(), j->second.as<std::string>()));
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
    "import { HttpContextContract } from '@ioc:Adonis/Core/HttpContext'\nimport Env from '@ioc:Adonis/Core/Env'\nimport { Filters } from 'App/Services/Filters'\nimport Logger from '@ioc:Adonis/Core/Logger'\nimport " + _crud_name + "Validator from '" + import_validator_folder + "/" + _crud_name + "Validator'\nimport " + _crud_name + " from 'App/Models/" + _crud_name + "'\nimport { ERRORS } from 'App/Exceptions/Messages'";
    std::string fileName = _crud_name + "sController.ts";
    std::string ControllerInit = "\n\nexport default class " + _crud_name + "sController {";
    std::string ControllerIndex = "\n  public async index({ response, params, request }: HttpContextContract) {\n    try {\n      const { ...details } = await request.validate(" + _crud_name + "Validator)\n      return response.json()\n    } catch (e) {\n      Logger.error({ err: Error }, e)\n      return response.abort({ errors: [{ message: ERRORS.MESSAGES.INDEX }] })\n    }\n  }";
    std::string ControllerShow = "\n\n  public async show({ response, params, request }: HttpContextContract) {\n    try {\n      const { ...details } = await request.validate(" + _crud_name + "Validator)\n      return response.json()\n    } catch (e) {\n      Logger.error({ err: Error }, e)\n      return response.abort({ errors: [{ message: ERRORS.MESSAGES.SHOW }] })\n    }\n  }";
    std::string ControllerPost = "\n\n  public async store({ response, params, request }: HttpContextContract) {\n    try {\n      const { ...details } = await request.validate(" + _crud_name + "Validator)\n      return response.json()\n    } catch (e) {\n      Logger.error({ err: Error }, e)\n      return response.abort({ errors: [{ message: ERRORS.MESSAGES.STORE }] })\n    }\n  }";
    std::string ControllerUpdate = "\n\n  public async update({ response, params, request }: HttpContextContract) {\n    try {\n      const { ...details } = await request.validate(" + _crud_name + "Validator)\n      return response.json()\n    } catch (e) {\n      Logger.error({ err: Error }, e)\n      return response.abort({ errors: [{ message: ERRORS.MESSAGES.UPDATE }] })\n    }\n  }";
    std::string ControllerArchive = "\n\n  public async archive({ response, params, request }: HttpContextContract) {\n    try {\n      const { ...details } = await request.validate(" + _crud_name + "Validator)\n      return response.json()\n    } catch (e) {\n      Logger.error({ err: Error }, e)\n      return response.abort({ errors: [{ message: ERRORS.MESSAGES.ARCHIVE }] })\n    }\n  }";
    std::string ControllerDelete = "\n\n  public async destroy({ response, params, request }: HttpContextContract) {\n    try {\n      const { ...details } = await request.validate(" + _crud_name + "Validator)\n      return response.json()\n    } catch (e) {\n      Logger.error({ err: Error }, e)\n      return response.abort({ errors: [{ message: ERRORS.MESSAGES.DELETE }] })\n    }\n  }";
    std::string ControllerEnd =  "\n}\n";

    if (route_crud == true) {
        checkFiles(fileName, "app/Controllers/Http");
        std::ofstream controller("app/Controllers/Http/" + fileName);
        controller << headers << ControllerInit;
        if (route_crud)
            controller << ControllerIndex << ControllerShow << ControllerPost << ControllerUpdate << ControllerArchive << ControllerDelete;
        controller << ControllerEnd;
        checkFiles(fileName, "app/Controllers/Http/Admin");
        std::ofstream controllerAdm("app/Controllers/Http/Admin/" + fileName);
        controllerAdm << headers << ControllerInit;
        if (route_crud)
            controllerAdm << ControllerIndex << ControllerShow << ControllerPost << ControllerUpdate << ControllerArchive << ControllerDelete;
        controllerAdm << ControllerEnd;
    }
}

void Resource::writeModel()
{
    std::string headers =
    "import { DateTime } from 'luxon'\nimport { BaseModel, column, belongsTo, BelongsTo, manyToMany, ManyToMany } from '@ioc:Adonis/Lucid/Orm'";
    std::string fileName = _crud_name + ".ts";
    std::string classInit = "\n\nexport default class " + _crud_name + " extends BaseModel {";
    std::string classEnd = "}\n";
    std::string baseFields = "\n  @column({ isPrimary: true })\n  public id: number\n";

    checkFiles(fileName, "app/Models");

    std::ofstream model("app/Models/" + fileName);
    model << headers;
    if (!foreign_keys.empty()) {
        for (std::map<std::string, std::string>::iterator it = foreign_keys.begin(); it != foreign_keys.end(); it++) {
            model << "\nimport " + it->second + " from 'App/Models/" + it->second + "'";
        }
    }
    if (!foreign_keys.empty()) {
        for (std::map<std::string, std::string>::iterator it = pivot_tables.begin(); it != pivot_tables.end(); it++) {
            model << "\nimport " + it->second + " from 'App/Models/" + it->second + "'";
        }
    }
    model << classInit << baseFields;
    for (std::map<std::string, std::string>::iterator it = _column.begin(); it != _column.end(); it++) {
        model << "\n  @column()\n  public " + it->first + ": " + it->second + "\n";
    }
    model << "\n  @column()\n  public archived: boolean\n\n  @column.dateTime({ autoCreate: true })\n  public createdAt: DateTime\n\n  @column.dateTime({ autoCreate: true, autoUpdate: true })\n  public updatedAt: DateTime";
    if (!foreign_keys.empty()) {
        for (std::map<std::string, std::string>::iterator it = foreign_keys.begin(); it != foreign_keys.end(); it++) {
            std::string secondLower = it->second;
            std::for_each(secondLower.begin(), secondLower.end(), [](char & c){
                c = ::tolower(c);
            });
            model << "\n\n  @belongsTo(() => " + it->second + ", {\n    foreignKey: '" + it->first + "',\n  })\n  public " + secondLower << ": BelongsTo<typeof " + it->second + ">";
        }
    }
    if (!pivot_tables.empty()) {
        for (std::map<std::string, std::string>::iterator it = pivot_tables.begin(); it != pivot_tables.end(); it++) {
            std::string secondLower = it->second;
            std::for_each(secondLower.begin(), secondLower.end(), [](char & c){
                c = ::tolower(c);
            });
            model << "\n\n  @manyToMany(() => " + it->second + ")\n  public " + it->first + ": ManyToMany<typeof " + it->second << ">";
        }
    }
    model << "\n" << classEnd;
}

void Resource::writePolicy()
{
    std::string headers = "import { BasePolicy } from '@ioc:Adonis/Addons/Bouncer'\nimport " + _crud_name + " from 'App/Models/" + _crud_name + "'";
    std::string fileName = _crud_name + "Policy.ts";
    std::string hasAccess = "\n\nasync function hasAccess() {\n  return await " + _crud_name + ".query()\n}\n\n";
    std::string classWrite = "export default class " + _crud_name + "Policy extends BasePolicy {\n  public async before() {\n    return true\n  }\n\n  public async index() {\n    return true\n  }\n\n  public async show() {\n    return true\n  }\n\n  public async store() {\n    return true\n  }\n\n  public async update() {\n    return true\n  }\n\n  public async destroy() {\n    return true\n  }\n}\n";

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
    std::string classInit = "\n\nexport default class " + _crud_name + "Validator {\n  constructor(protected ctx: HttpContextContract) {}\n\n  public schema = schema.create({\n";
    std::string classEnd =  "  })\n\n  public messages = {}\n}\n";
    std::string trim = "{}";

    if (!(std::filesystem::is_directory("app/Validators/" + _crud_name + 's')))
        std::filesystem::create_directory("app/Validators/" + _crud_name + 's');
    checkFiles(fileName, "app/Validators/" + _crud_name + 's');

    std::ofstream validator("app/Validators/" + _crud_name + "s/" + fileName);
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
        validator << "    " + it->first + ": schema." + it->second + opt + "(" + trim + ", []),\n";
        trim = "{}";
    }
    validator << classEnd;
}

void Resource::writeRoutes()
{
    std::string headers =
    "import Route from '@ioc:Adonis/Core/Route'\n\n";
    std::string fileName = _crud_lower + "s.ts";
    std::string RoutesInit = "/**\n * " + _crud_name + "s\n */\nRoute.group(() => {\n  Route.group(() => {\n    ";
    std::string RoutesEnd =  "}).middleware(['auth'])\n";

    if (route_crud == true) {
        checkFiles(fileName, "start/routes");
        std::ofstream routes("start/routes/" + fileName);
        routes << headers << RoutesInit;
        if (route_crud)
            routes << "Route.resource('" + _crud_lower + "s', 'Admin/" + _crud_name + "sController')\n  })\n    .middleware('admin')\n    .prefix('admin')\n\n  Route.resource('" + _crud_lower + "s', '" + _crud_name + "sController')\n";
        for (std::map<std::string, std::string>::iterator it = additional_routes.begin(); it != additional_routes.end(); it++)
            routes << "  Route." + it->second + "('" + it->first + "', '" + _crud_name + "sController')\n";
        routes << RoutesEnd;

        std::ofstream routesIndex;
        routesIndex.open("start/routes.ts", std::ios_base::app);
        routesIndex << "import './routes/" + _crud_lower + "s'\n";
    }
}

void Resource::writeSeeder()
{
    std::string headers =
    "import BaseSeeder from '@ioc:Adonis/Lucid/Seeder'\nimport " + _crud_name + " from 'App/Models/" + _crud_name + "'";
    std::string fileName = _crud_name + ".ts";
    std::string classInit = "\n\nexport default class " + _crud_name + "Seeder extends BaseSeeder {\n  public async run() {\n    ";
    std::string fillInit = "const " + _crud_lower + " = await " + _crud_name + ".create({\n";
    std::string classEnd =  "    })\n  }\n}\n";

    checkFiles(fileName, "database/seeders");
    if (_tests) {
        std::ofstream seeder("database/seeders/" + fileName);
        seeder << headers << classInit << fillInit;
        for (std::map<std::string, std::string>::iterator it = _column.begin(); it != _column.end(); it++) {
            std::string str = "other_types";
            int num = 0;
            bool bl = false;
            if (it->second == "number")
                seeder << "      " << it->first << ": " << num << ",\n";
            else if (it->second == "Boolean" || it->second == "boolean" || it->second == "bool")
                seeder << "      " << it->first << ": " << bl << ",\n";
            else
                seeder << "      " << it->first << ": '" << str << "',\n";
        }
        seeder << classEnd;
    }
}

void Resource::writeTests()
{
    std::string headers = "import test from 'japa'\nimport supertest from 'supertest'";
    std::string fileName = _crud_name + ".spec.ts";
    std::string consts = "\n\nconst BASE_URL = `http://${process.env.HOST}:${process.env.PORT}`\n\n";
    std::string testsInit = "test.group('test API " + _crud_name + " controller', async () => {";
    std::string loginTests = "\n  /**\n   * Login\n   */\n  test.group('test login', async () => {})\n";
    std::string crudTests = "\n  /**\n   * " + _crud_name + "\n   */\n  test.group('test " + _crud_lower + "', async () => {})\n";
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
