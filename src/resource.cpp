#include "resource.hpp"

Resource::Resource()
{
    dir_to_check = {"app", "app/Controllers", "app/Controllers/Http", "app/Models",
    "app/Validators", "app/Policies", "start", "start/routes"};
}

int Resource::core() {
    YAML::Node file;

    file = openFile();
    if (readFile(file) != 0) {
        return -1;
    }
    for (size_t i = 0; i < dir_to_check.size(); i++) {
        if (checkDirectory(dir_to_check[i]) != 0) {
            return -1;
        }
    }
    if (writeModel() != 0) {
        return -1;
    }
    if (writeValidator() != 0) {
        return -1;
    }
    if (writePolicy() != 0) {
        return -1;
    }
    if (writeRoutes() != 0) {
        return -1;
    }
    if (writeController() != 0) {
        return -1;
    }
    return 0;
}

YAML::Node Resource::openFile() {
    YAML::Node file;

    try {
        file = YAML::LoadFile("./deus-config/deus-config.yml");
        std::cout << "File loaded" << std::endl;
    } catch(YAML::BadFile &e) {
        std::cerr << "File/Path error" << std::endl;
        EXIT_FAILURE;
    }
    return file;
}

int Resource::readFile(YAML::Node file) {
    try {
        if (!file["crud-name"] || file["crud-name"].as<std::string>().empty()) {
            // null variable check doesn't work
            throw "Crud name is blank";
        }
        const std::string crudName = file["crud-name"].as<std::string>();
        setCrudName(crudName);
        std::cout << "Crud name setted: " << _crud_name << std::endl;

        const YAML::Node& models = file["models"];
        for (std::size_t i = 0; i < models.size(); i++) {
            const YAML::Node model = models[i];
            for(auto j = model.begin(); j != model.end(); j++) {
                _column.emplace(std::make_pair(j->first.as<std::string>(), j->second.as<std::string>()));
                std::cout << "Name: " << j->first.as<std::string>() << std::endl;
                std::cout << "Type: " << j->second.as<std::string>() << std::endl;
            }
        }

        const YAML::Node& foreign = file["foreign-keys"];
        for (std::size_t i = 0; i < foreign.size(); i++) {
            const YAML::Node key = foreign[i];
            for(auto j = key.begin(); j != key.end(); j++) {
                foreign_keys.emplace(std::make_pair(j->first.as<std::string>(), j->second.as<std::string>()));
                std::cout << "Key: " << j->first.as<std::string>() << std::endl;
                std::cout << "Model: " << j->second.as<std::string>() << std::endl;
            }
        }

        const YAML::Node &optional = file["body-validator-optional-values"];
        for(YAML::const_iterator it = optional.begin(); it != optional.end(); it++) {
            const YAML::Node &opt = *it;
            validator_optional.push_back(opt.as<std::string>());
            std::cout << "Optional: " << opt.as<std::string>() << std::endl;
        }

        const YAML::Node &controller = file["controller"];
        for (std::size_t i = 0; i < controller.size(); i++) {
            const YAML::Node route = controller[i];
            if (controller[i]["crud"]) {
                route_crud = controller[i]["crud"].as<bool>();
                std::cout << "Crud-route: " << route_crud << std::endl;
            }
            if (controller[i]["additionals"]) {
                for (std::size_t j = 0; j < controller[i]["additionals"].size(); j++)
                {
                    additional_routes.emplace(std::make_pair(controller[i]["additionals"][j]["routes"].as<std::string>(), controller[i]["additionals"][j]["http"].as<std::string>()));
                    std::cout << "Additional-routes: " << controller[i]["additionals"][j]["routes"] << std::endl;
                    std::cout << "Additional-http: " << controller[i]["additionals"][j]["http"] << std::endl;
                }
            }
        }

        if (!file["policies"] || file["policies"].as<std::string>().empty()) {
            // null variable check doesn't work
            throw "Policies choice is blank";
        }
        const bool policies = file["policies"].as<bool>();
        setPolicies(policies);
        std::cout << "Policies : " << policies << std::endl;

        if (!file["automated-routes-testing"] || file["automated-routes-testing"].as<std::string>().empty()) {
            // null variable check doesn't work
            throw "Tests choice is blank";
        }
        const bool tests = file["automated-routes-testing"].as<bool>();
        setTests(tests);
        std::cout << "Tests choice : " << _tests << std::endl;

        return 0;
    } catch(const char* error) {
        std::cerr << "Read error: " << error << std::endl;
        return -1;
    }
}

int Resource::checkDirectory(std::string dir) {
    if (!std::filesystem::is_directory(dir)) {
        std::filesystem::create_directory(dir);
        if (!std::filesystem::is_directory(dir)) {
            return -1;
        }
    }
    return 0;
}

int Resource::checkFiles(std::string file, std::string path) {
    std::filesystem::current_path(path);
    if (std::filesystem::exists(file)) {
        return -1;
    }
    return 0;
}

void Resource::resetPath() {
    std::filesystem::current_path("../../");
    return;
}

void Resource::setCrudName(std::string _var) {
    _crud_name = _var;
}

void Resource::setPolicies(bool _var) {
    _policies = _var;
}

void Resource::setTests(bool _var) {
    _tests = _var;
}

int Resource::writeController() {
    try {
        std::string headers =
        "import { HttpContextContract } from '@ioc:Adonis/Core/HttpContext'\nimport Env from '@ioc:Adonis/Core/Env'\nimport { Filters } from 'App/Services/Filters'\nimport Logger from '@ioc:Adonis/Core/Logger'\nimport " + _crud_name + "Validator from 'App/Validators/" + _crud_name + "Validator'";
        std::string fileName = _crud_name + "Controller.ts";
        std::string ControllerInit = "\n\nexport default class " + _crud_name + "Controller {\n\tpublic initQuery() {\n\t\treturn " + _crud_name + ".query()\n\t}";
        std::string ControllerGetAll = "\n\n\tpublic async getAll({ response, params, request }: HttpContextContract) {\n\t\ttry {\n\t\t\tconst details = await.request.validate(" + _crud_name + "Validator)\n\t\t\treturn\n\t\t} catch (e) {\n\t\t\tLogger.error({ err: Error }, e)\n\t\t\treturn response.abort({ errors: [{ message: '" + _crud_name + " GetAll error' }] })\n\t\t}\n\t}";
        std::string ControllerGet = "\n\n\tpublic async get({ response, params, request }: HttpContextContract) {\n\t\ttry {\n\t\t\tconst details = await.request.validate(" + _crud_name + "Validator)\n\t\t\treturn\n\t\t} catch (e) {\n\t\t\tLogger.error({ err: Error }, e)\n\t\t\treturn response.abort({ errors: [{ message: '" + _crud_name + " Get error' }] })\n\t\t}\n\t}";
        std::string ControllerPost = "\n\n\tpublic async post({ response, params, request }: HttpContextContract) {\n\t\ttry {\n\t\t\tconst details = await.request.validate(" + _crud_name + "Validator)\n\t\t\treturn\n\t\t} catch (e) {\n\t\t\tLogger.error({ err: Error }, e)\n\t\t\treturn response.abort({ errors: [{ message: '" + _crud_name + " Post error' }] })\n\t\t}\n\t}";
        std::string ControllerUpdate = "\n\n\tpublic async update({ response, params, request }: HttpContextContract) {\n\t\ttry {\n\t\t\tconst details = await.request.validate(" + _crud_name + "Validator)\n\t\t\treturn\n\t\t} catch (e) {\n\t\t\tLogger.error({ err: Error }, e)\n\t\t\treturn response.abort({ errors: [{ message: '" + _crud_name + " Update error' }] })\n\t\t}\n\t}";
        std::string ControllerDelete = "\n\n\tpublic async delete({ response, params, request }: HttpContextContract) {\n\t\ttry {\n\t\t\tconst details = await.request.validate(" + _crud_name + "Validator)\n\t\t\treturn\n\t\t} catch (e) {\n\t\t\tLogger.error({ err: Error }, e)\n\t\t\treturn response.abort({ errors: [{ message: '" + _crud_name + " Delete error' }] })\n\t\t}\n\t}";
        std::string ControllerEnd =  "\n}\n";

        if (checkFiles(fileName, "app/Controllers/Http") != 0) {
            throw "Controller already exists";
        }
        std::ofstream controller(fileName);
        controller << headers << ControllerInit;
        if (route_crud) {
            controller << ControllerGetAll << ControllerGet << ControllerPost << ControllerUpdate << ControllerDelete;
        }
        controller << ControllerEnd;
        controller.close();
        resetPath();
        return 0;
    } catch(const char* error) {
        std::cerr << error << std::endl;
        return -1;
    }
}

int Resource::writeModel() {
    try {
        std::string headers =
        "import { DateTime } from 'luxon'\nimport { BaseModel, column, belongsTo, BelongsTo } from '@ioc:Adonis/Lucid/Orm'";
        std::string fileName = _crud_name + ".ts";
        std::string classInit = "\n\nexport default class " + _crud_name + " extends BaseModel {";
        std::string classEnd = "}\n";
        std::string baseFields = "\n\t@column({ isPrimary: true })\n\tpublic id: number\n\n\t@column.dateTime({ autoCreate: true })\n\tpublic createdAt: DateTime\n\n\t@column.dateTime({ autoCreate: true, autoUpdate: true })\n\tpublic updatedAt: DateTime\n";

        if (checkFiles(fileName, "app/Models") != 0) {
            throw "Model already exists";
        }
        std::ofstream model(fileName);
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
        model << "\n";
        if (!foreign_keys.empty()) {
            for (std::map<std::string, std::string>::iterator it = foreign_keys.begin(); it != foreign_keys.end(); it++) {
                std::string secondLower = it->second;
                std::for_each(secondLower.begin(), secondLower.end(), [](char & c){
                    c = ::tolower(c);
                });
                model << "\t@belongsTo(() => " + it->second + ", {\n\t\tforeignKey: '" + it->first + "'\n\t})\n\tpublic " + secondLower << ": BelongsTo<typeof " + it->second + ">\n";
            }
        }
        model << classEnd;
        model.close();
        resetPath();
        return 0;
    } catch(const char* error) {
        std::cerr << error << std::endl;
        return -1;
    }
}

int Resource::writePolicy() {
    try {
        std::string headers = "import { BasePolicy } from '@ioc:Adonis/Addons/Bouncer'\nimport " + _crud_name + " from 'App/Models/" + _crud_name + "'";
        std::string fileName = _crud_name + "Policy.ts";
        std::string hasAccess = "\n\nasync function hasAccess() {\n\treturn (\n\t\t(await " + _crud_name + ".query())\n\t)\n}\n\n";
        std::string classWrite = "export default class " + _crud_name + "Policy extends BasePolicy {\n\tpublic async before() {\n\t\treturn\n\t}\n\n\tpublic async index() {\n\t\treturn\n\t}\n\n\tpublic async show() {\n\t\treturn\n\t}\n\n\tpublic async store() {\n\t\treturn\n\t}\n\n\tpublic async update() {\n\t\treturn\n\t}\n\n\tpublic async destroy() {\n\t\treturn\n\t}\n}\n";

        if (checkFiles(fileName, "app/Policies") != 0) {
            throw "Policy already exists";
        }
        if (_policies) {
            std::ofstream policy(fileName);
            policy << headers << hasAccess << classWrite;
            policy.close();
        }
        resetPath();
        return 0;
    } catch(const char* error) {
        std::cerr << error << std::endl;
        return -1;
    }
}

int Resource::writeValidator() {
    try {
        std::string headers =
        "import { schema, rules } from '@ioc:Adonis/Core/Validator'\nimport { HttpContextContract } from '@ioc:Adonis/Core/HttpContext'";
        std::string fileName = _crud_name + "Validator.ts";
        std::string classInit = "\n\nexport default class " + _crud_name + "Validator {\n\tconstructor(protected ctx: HttpContextContract) {}\n\n\tpublic schema = schema.create({\n";
        std::string classEnd =  "\t})\n\n\tpublic messages = {}\n}\n";

        if (checkFiles(fileName, "app/Validators") != 0) {
            throw "Validator already exists";
        }
        std::ofstream validator(fileName);
        validator << headers << classInit;
        for (std::map<std::string, std::string>::iterator it = _column.begin(); it != _column.end(); it++) {
            std::string opt = "";
            for (std::size_t i = 0; i < validator_optional.size(); i++)
            {
                if (validator_optional[i] == it->first) {
                    opt = ".optional";
                }
            }
            validator << "\t\t" + it->first + ": schema." + it->second + opt + "({}, []),\n";
        }
        validator << classEnd;
        validator.close();
        resetPath();
        return 0;
    } catch(const char* error) {
        std::cerr << error << std::endl;
        return -1;
    }
}

int Resource::writeRoutes() {
    try {
        std::string crudLower = _crud_name;
        std::for_each(crudLower.begin(), crudLower.end(), [](char & c){
            c = ::tolower(c);
        });
        std::string headers =
        "import Route from '@ioc:Adonis/Core/Route'\n\n";
        std::string fileName = crudLower + ".ts";
        std::string RoutesInit = "/**\n * " + _crud_name + "\n */\nRoute.group(() => {\n\t";
        std::string RoutesEnd =  "}).middleware(['auth'])\n";

        if (checkFiles(fileName, "start/routes") != 0) {
            throw "Routes already exists";
        }
        std::ofstream routes(fileName);
        routes << headers << RoutesInit;
        if (route_crud) {
            routes << "Route.resource('" + crudLower + "', '" + _crud_name + "Controller')\n";
        }
        for (std::map<std::string, std::string>::iterator it = additional_routes.begin(); it != additional_routes.end(); it++) {
            routes << "\tRoutes." + it->second + "('" + it->first + "', '" + _crud_name + "Controller')\n";
        }
        routes << RoutesEnd;
        routes.close();
        resetPath();
        return 0;
    } catch(const char* error) {
        std::cerr << error << std::endl;
        return -1;
    }
}
